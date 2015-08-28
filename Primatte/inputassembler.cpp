#include "inputassembler.h"
#include "matrixd.h"
#include "io.h"
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include "iaveragebackgroundcolourlocator.h"

namespace anima
{
    namespace ia
    {
        std::vector<math::vec3> RemoveDuplicatesWithGrid(const cv::Mat& mat, unsigned gridSize)
        {
            START_TIMER(CleaningWithGrid);

            assert(mat.type() == CV_32FC3);
            const unsigned r = mat.rows, c = mat.cols;

            std::vector<math::vec3> points;
            points.reserve(r*c/50);

            const unsigned gridCubed = gridSize*gridSize*gridSize;
            const unsigned gridSizeMinusOne = gridSize-1;

            bool* grid = new(std::nothrow) bool[gridCubed]();

            if(!grid)
                throw std::runtime_error("Unable to allocate 3D grid for input processing");

            for (unsigned i = 0; i < r; ++i)
            {
                float* data = (float*)(mat.data + mat.step*i);
                for(unsigned j = 0; j < c; ++j)
                {
                    math::vec3& p = *((math::vec3*)(data + j*3));
                    math::vec3i pi(p.x*gridSize,p.y*gridSize,p.z*gridSize);
                    if(unsigned(pi.x) >= gridSize)
                        pi.x = gridSizeMinusOne;
                    if(unsigned(pi.y) >= gridSize)
                        pi.y = gridSizeMinusOne;
                    if(unsigned(pi.z) >= gridSize)
                        pi.z = gridSizeMinusOne;

                    bool& b = grid[pi.x + gridSize*(pi.y + gridSize*pi.z)];
                    if (!b)
                    {
                        b = true;
                        points.push_back(p);
                    }
                }
            }

            delete[] grid;

            END_TIMER(CleaningWithGrid);

            return points;
        }

        void RandomSimplify(std::vector<math::vec3>* points, float percentageToRemove)
        {
            START_TIMER(RandomSimplifying);
            size_t initialSize = points->size();

            std::random_device rd;
            std::mt19937 random(rd());
            std::shuffle(points->begin(), points->end(), random);

            points->erase(points->begin()+(int)(points->size()*(1.f-percentageToRemove/100.f)), points->end());

            Inform("" + ToString(points->size()/float(initialSize)*100) + "% of points remain (" +
                   ToString(points->size()) + "/" + ToString(initialSize)+")");

            END_TIMER(RandomSimplifying);
        }

        float InputAssembler::normalisationMultiplier(int cvCode)
        {
            switch(cvCode)
            {
            case CV_8UC3:
                return 1.0/255.0;
            case CV_16UC3:
                return 1.0/65535.0;
            case CV_32FC3:
                return 1.0;
            default:
                throw std::runtime_error("Unknown format in input assember at line " + ToString(__LINE__));
            }
            assert(0);
        }

        InputAssembler::InputAssembler(InputAssemblerDescriptor& desc)
        {
            START_TIMER(ProcessingInput);
            //Convert the input into 3 component float mat.
            if(desc.foregroundSource == nullptr)
                throw std::runtime_error("Null source foreground Mat");

            if(desc.backgroundSource == nullptr)
                throw std::runtime_error("Null source background mat");

            if(desc.backgroundLocator == nullptr)
                throw std::runtime_error("Null background colour locator");

            if(!desc.ipd.validate())
                throw std::runtime_error("Could not validate input processor.");

            if(desc.foregroundSource->cols*desc.foregroundSource->rows==0)
                throw std::runtime_error("Empty foreground source.");

            if(desc.backgroundSource->cols*desc.backgroundSource->rows==0)
                throw std::runtime_error("Empty background source.");


            desc.foregroundSource->convertTo(mForegroundF, CV_32FC3,
                                             normalisationMultiplier(desc.foregroundSource->type()));
            desc.backgroundSource->convertTo(mBackgroundF, CV_32FC3,
                                             normalisationMultiplier(desc.backgroundSource->type()));

            //Convert everything to the appropriate colour space:
            mColourSpace = desc.targetColourspace;

            switch(desc.targetColourspace)
            {
            case InputAssemblerDescriptor::ETCS_RGB:
                break;
            case InputAssemblerDescriptor::ETCS_HSV:
                cv::cvtColor(mForegroundF, mForegroundF, CV_RGB2HSV);
                cv::cvtColor(mBackgroundF, mBackgroundF, CV_RGB2HSV);

                //Normalise hue:
                for (int i = 0; i < mForegroundF.rows; ++i)
                {
                    float* data = (float*)(mForegroundF.data + mForegroundF.step*i);
                    for(int j = 0; j < mForegroundF.cols; ++j)
                        *(data + j*3)/=360.f;
                }
                for (int i = 0; i < mBackgroundF.rows; ++i)
                {
                    float* data = (float*)(mBackgroundF.data + mBackgroundF.step*i);
                    for(int j = 0; j < mBackgroundF.cols; ++j)
                        *(data + j*3)/=360.f;
                }
                break;
            case InputAssemblerDescriptor::ETCS_LAB:
                cv::cvtColor(mForegroundF, mForegroundF, CV_RGB2Lab);
                cv::cvtColor(mBackgroundF, mBackgroundF, CV_RGB2Lab);

                //Get into proper range
                for (int i = 0; i < mForegroundF.rows; ++i)
                {
                    float* data = (float*)(mForegroundF.data + mForegroundF.step*i);
                    for(int j = 0; j < mForegroundF.cols; ++j)
                    {
                        math::vec3& p = *((math::vec3*)(data + j*3));
                        p = math::vec3(p.x, (p.y+127.f), (p.z+127.f))/254.f;
                    }
                }
                for (int i = 0; i < mBackgroundF.rows; ++i)
                {
                    float* data = (float*)(mBackgroundF.data + mBackgroundF.step*i);
                    for(int j = 0; j < mBackgroundF.cols; ++j)
                    {
                        math::vec3& p = *((math::vec3*)(data + j*3));
                        p = math::vec3(p.x, (p.y+127.f), (p.z+127.f))/254.f;
                    }
                }
                break;
            }

            //Convert mat to vector, and clean:
            mPoints = RemoveDuplicatesWithGrid(mForegroundF, desc.ipd.gridSize);
            mBackgroundPoints = RemoveDuplicatesWithGrid(mBackgroundF, desc.ipd.gridSize);

            //Find dominant background colour:
            mBackground = desc.backgroundLocator->findColour(mBackgroundF);

            //Simplify randomly if needed:
            if(desc.ipd.randomSimplify)
            {
                RandomSimplify(&mPoints, desc.ipd.randomSimplifyPercentage);
                RandomSimplify(&mBackgroundPoints, desc.ipd.randomSimplifyPercentage);
            }

            END_TIMER(ProcessingInput);
        }

        cv::Point3f InputAssembler::debugGetPointColour(math::vec3 p) const
        {
            switch(mColourSpace)
            {
            case InputAssemblerDescriptor::ETCS_RGB:
                break;
            case InputAssemblerDescriptor::ETCS_HSV:
            {
                p.x *= 360.f;
                cv::Mat mat(1,1,CV_32FC3, &p.x);
                cv::cvtColor(mat, mat, CV_HSV2BGR);
            }
                break;
            case InputAssemblerDescriptor::ETCS_LAB:
            {
                p = math::vec3(p.x*254.f,
                                p.y*254.f - 127.f,
                                p.z*254.f - 127.f);

                cv::Mat mat(1,1,CV_32FC3, &p.x);
                cv::cvtColor(mat, mat, CV_Lab2BGR);
            }
                break;
            default:
                assert(0);
            }

            return cv::Point3f(p.z,p.y,p.x);
        }

        const std::vector<math::vec3>& InputAssembler::backgroundPoints() const
        {
            return mBackgroundPoints;
        }

        const std::vector<math::vec3>& InputAssembler::points() const
        {
            return mPoints;
        }

        const cv::Mat& InputAssembler::mat() const
        {
            return mForegroundF;
        }

        math::vec3 InputAssembler::background() const
        {
            return mBackground;
        }
    }
}
