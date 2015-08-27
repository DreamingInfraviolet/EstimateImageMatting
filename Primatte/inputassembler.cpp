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
                for(unsigned j = 0; j < c; ++j)
                {
                    cv::Point3f p = mat.at<cv::Point3f>(i,j);
                    cv::Point3i pi = p*(int)gridSize;
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
                        points.push_back(math::vec3(p.x, p.y, p.z));
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


            double foregroundMultiplier, backgroundMultiplier;
            switch(desc.foregroundSource->type())
            {
            case CV_8UC3:
                foregroundMultiplier = 1.0/255.0;
                break;
            case CV_16UC3:
                foregroundMultiplier = 1.0/65535.0;
                break;
            case CV_32FC3:
                foregroundMultiplier = 1.0;
                break;
            default:
                throw std::runtime_error("Unknown format in input assember at line " + ToString(__LINE__));
            }
            switch(desc.backgroundSource->type())
            {
            case CV_8UC3:
                backgroundMultiplier = 1.0/255.0;
                break;
            case CV_16UC3:
                backgroundMultiplier = 1.0/65535.0;
                break;
            case CV_32FC3:
                backgroundMultiplier = 1.0;
                break;
            default:
                throw std::runtime_error("Unknown format in input assember at line " + ToString(__LINE__));
            }

            desc.foregroundSource->convertTo(mForegroundF, CV_32FC3, foregroundMultiplier);
            desc.backgroundSource->convertTo(mBackgroundF, CV_32FC3, backgroundMultiplier);

            //Convert to appropriate colour space (including background pixel):
            mColourSpace = desc.targetColourspace;

            switch(desc.targetColourspace)
            {
            case InputAssemblerDescriptor::ETCS_RGB:
                break;
            case InputAssemblerDescriptor::ETCS_HSV:
                cv::cvtColor(mForegroundF, mForegroundF, CV_RGB2HSV);
                cv::cvtColor(mBackgroundF, mBackgroundF, CV_RGB2HSV);

                //Normalise hue:
                for(int y = 0; y < mForegroundF.cols; ++y)
                    for(int x = 0; x < mForegroundF.rows; ++x)
                        mForegroundF.at<cv::Point3f>(x,y).x/=360.f;

                for(int y = 0; y < mBackgroundF.cols; ++y)
                    for(int x = 0; x < mBackgroundF.rows; ++x)
                        mBackgroundF.at<cv::Point3f>(x,y).x/=360.f;
                break;
            case InputAssemblerDescriptor::ETCS_LAB:
                cv::cvtColor(mForegroundF, mForegroundF, CV_RGB2Lab);
                cv::cvtColor(mBackgroundF, mBackgroundF, CV_RGB2Lab);

                for(int y = 0; y < mForegroundF.cols; ++y)
                    for(int x = 0; x < mForegroundF.rows; ++x)
                    {
                        cv::Point3f& p = mForegroundF.at<cv::Point3f>(x,y);
                        p = cv::Point3f(p.x/254.f, (p.y+127.f)/254.f, (p.z+127.f)/254.f);
                    }
                for(int y = 0; y < mBackgroundF.cols; ++y)
                    for(int x = 0; x < mBackgroundF.rows; ++x)
                    {
                        cv::Point3f& p = mBackgroundF.at<cv::Point3f>(x,y);
                        p = cv::Point3f(p.x/254.f, (p.y+127.f)/254.f, (p.z+127.f)/254.f);
                    }
                break;
            }

            mPoints = RemoveDuplicatesWithGrid(mForegroundF, desc.ipd.gridSize);
            mBackgroundPoints = RemoveDuplicatesWithGrid(mBackgroundF, desc.ipd.gridSize);

            mBackground = desc.backgroundLocator->findColour(mBackgroundF);


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
                cv::cvtColor(mat, mat, CV_HSV2RGB);
            }
                break;
            case InputAssemblerDescriptor::ETCS_LAB:
            {
                p = math::vec3(p.x*254.f,
                                p.y*254.f - 127.f,
                                p.z*254.f - 127.f);

                cv::Mat mat(1,1,CV_32FC3, &p.x);
                cv::cvtColor(mat, mat, CV_Lab2RGB);
            }
                break;
            default:
                assert(0);
            }

            return cv::Point3f(p.x,p.y,p.z);
        }

        const std::vector<math::vec3> &InputAssembler::points() const
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
