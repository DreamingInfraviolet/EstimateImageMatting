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
        public:

            /** Initialises the input, throwing an exception if failed. */
            InputAssembler(InputAssemblerDescriptor& desc);

            /** Returns the internal points. */
            const std::vector<alg::Point>& points() const;

            /** Loads a cv::Mat from an image file in rgb format. */
            static cv::Mat loadMatFromFile(const char *path);


            /** Converts the given RGB pixels to hsv. */
            static void rgbToHsv(cv::Mat* rgb);

            /** Helper function */
            static cv::Mat loadRgbMatFromFile(const char* path);
        };


    }
}
