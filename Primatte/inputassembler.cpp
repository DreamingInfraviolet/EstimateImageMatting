#include "inputassembler.h"
#include "matrixd.h"
#include "io.h"
#include <set>

namespace anima
{
    namespace ia
    {

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
            //Convert to appropriate colour space:
            switch(desc.targetColourspace)
            {
            case InputAssemblerDescriptor::ETCS_RGB:
                break;
            case InputAssemblerDescriptor::ETCS_HSV:
                cv::cvtColor(mMatF, mMatF, CV_RGB2HSV);

                //Normalise hue:
                for(int y = 0; y < mMatF.cols; ++y)
                    for(int x = 0; x < mMatF.rows; ++x)
                    {
                        cv::Point3f& p = mMatF.at<cv::Point3f>(x,y);
                        p.x/=360.f;
                    }
                break;
            }

            mPoints = ProcessPoints(mMatF, desc.ipd);
        }

        const std::vector<alg::Point>& InputAssembler::points() const
        {
            return mPoints;
        }
    }
}
