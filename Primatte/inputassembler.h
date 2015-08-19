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
        public:

            /** Initialises the input, throwing an exception if failed. */
            InputAssembler(InputAssemblerDescriptor& desc);

            /** Returns the internal points. */
            const std::vector<alg::Point>& points() const;

            /** Returns the internal floating point image. */
            const cv::Mat& mat() const;

            /** Returns the background point in the correct colour space. */
            alg::Point background() const;

            /** Loads a cv::Mat from an image file in rgb format. */
            static cv::Mat loadMatFromFile(const char *path);


            /** Converts the given RGB pixels to hsv. */
            static void rgbToHsv(cv::Mat* rgb);

            /** Helper function */
            static cv::Mat loadRgbMatFromFile(const char* path);
        };


    }
}
