#include "inputassembler.h"
#include "matrixd.h"
#include "io.h"
#include <set>

namespace anima
{
    namespace ia
    {

        InputAssembler::~InputAssembler()
        {
            delete[] mGrid;
        }

        cv::Mat InputAssembler::loadRgbMatFromFile(const char* path)
        {
            cv::Mat mat;
            mat = cv::imread(path);
            if(mat.data==nullptr)
                throw std::runtime_error("Could not load image data.");

            cv::Mat matbgr;
            mat.convertTo(matbgr, CV_8UC3);

            cv::Mat imageMat;
            cv::cvtColor(matbgr, imageMat, CV_BGR2RGB);

            return imageMat;
        }

        InputAssembler::InputAssembler(InputAssemblerDescriptor& desc)
            : mGrid(nullptr)
        {
            //Convert the input into 3 component float mat.
            if(desc.source == nullptr)
                throw std::runtime_error("Null source mat");

            double alpha;
            switch(desc.source->type())
            {
            case CV_8UC3:
                alpha = 1/255.f;
                break;
            case CV_16UC3:
                alpha = 1/65535.f;
                break;
            case CV_32FC3:
                alpha = 1.f;
                break;
            default:
                throw std::runtime_error("Unknown format in input assember at line " + ToString(__LINE__));
            }

            desc.source->convertTo(mMatF, CV_32FC3, alpha);

            //Put background pixel into mat for conversion
            float f[3];
            f[0] = desc.backgroundPoint.x();
            f[1] = desc.backgroundPoint.y();
            f[2] = desc.backgroundPoint.z();
            cv::Mat backgroundPxMat(1,1,CV_32FC3, f);

            //Convert to appropriate colour space (including background pixel):
            mColourSpace = desc.targetColourspace;

            switch(desc.targetColourspace)
            {
            case InputAssemblerDescriptor::ETCS_RGB:
                break;
            case InputAssemblerDescriptor::ETCS_HSV:
                cv::cvtColor(mMatF, mMatF, CV_RGB2HSV);
                cv::cvtColor(backgroundPxMat, backgroundPxMat, CV_RGB2HSV);

                //Normalise hue:
                for(int y = 0; y < mMatF.cols; ++y)
                    for(int x = 0; x < mMatF.rows; ++x)
                    {
                        cv::Point3f& p = mMatF.at<cv::Point3f>(x,y);
                        p.x/=360.f;
                    }
                backgroundPxMat.at<cv::Point3f>(0,0).x/=360.f;
                break;
            }

            //restore background pixel from mat in new colourspace
            cv::Point3f cvbgpx = backgroundPxMat.at<cv::Point3f>(0,0);
            mBackground = alg::Point(cvbgpx.x,cvbgpx.y,cvbgpx.z);

            auto result = ProcessPoints(mMatF, desc.ipd);
            mPoints = result.first;
            mGrid = result.second;
            mGridSize = desc.ipd.gridSize;
        }

        const std::vector<alg::Point>& InputAssembler::points() const
        {
            return mPoints;
        }

        const cv::Mat& InputAssembler::mat() const
        {
            return mMatF;
        }

        alg::Point InputAssembler::background() const
        {
            return mBackground;
        }
    }
}
