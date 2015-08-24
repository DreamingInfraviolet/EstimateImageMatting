#pragma once
#include <cstring>
#include <vector>
#include "inputprocessing.h"
#include <opencv2/opencv.hpp>
    /**
      * This class is responsible for taking an InputAssemblerDescriptor object,
      * and load+store the corresponding data.
      * */

namespace anima
{
    namespace ia
    {

        /** The descriptor of the input data/source. */
        struct InputAssemblerDescriptor
        {
            enum TargetColourspace {ETCS_RGB, ETCS_HSV};

            /** The colour space to which the input will be converted. */
            TargetColourspace targetColourspace;

            /** The source from which the input should be copied. */
            cv::Mat* source;

            InputProcessingDescriptor ipd;

            /** A normalised background point, in the colourspace of the source image.
              * Example, rgb(0,0,1), rgb(0.2,0.4,0) */
            alg::Point backgroundPoint;

            InputAssemblerDescriptor()
            {
                memset(this, 0, sizeof(*this));
            }
        };

        /** The input assembler class. */
        class InputAssembler
        {
            cv::Mat mMatF;
            std::vector<alg::Point> mPoints;
            alg::Point mBackground;
            bool* mGrid; //A 3D grid
            size_t mGridSize;
            InputAssemblerDescriptor::TargetColourspace mColourSpace;
        public:

            /** Initialises the input, throwing an exception if failed. */
            InputAssembler(InputAssemblerDescriptor& desc);

            ~InputAssembler();

            /** Returns the internal points. */
            const std::vector<alg::Point>& points() const;

            /** Returns the internal floating point image. */
            const cv::Mat& mat() const;

            /** Returns the background point in the correct colour space. */
            alg::Point background() const;

            const bool* grid() const {return mGrid;}

            size_t gridSize() const { return mGridSize; }

            //Converts from internal colour space to rgb
            cv::Point3f debugGetPointColour(alg::Point p) const
            {
                switch(mColourSpace)
                {
                case InputAssemblerDescriptor::ETCS_RGB:
                    return cv::Point3f(p.x(),p.y(),p.z());
                    break;
                case InputAssemblerDescriptor::ETCS_HSV:
                {
                    float f[3];
                    f[0] = p.x()*360.f;
                    f[1] = p.y();
                    f[2] = p.z();
                    cv::Mat mat(1,1,CV_32FC3, &f);
                    cv::cvtColor(mat, mat, CV_HSV2RGB);
                    return cv::Point3f(f[0],f[1],f[2]);
                }
                    break;
                default:
                    assert(0);
                }
            }

            /** Loads a cv::Mat from an image file in rgb format. */
            static cv::Mat loadMatFromFile(const char *path);


            /** Converts the given RGB pixels to hsv. */
            static void rgbToHsv(cv::Mat* rgb);

            /** Helper function */
            static cv::Mat loadRgbMatFromFile(const char* path);

        };


    }
}
