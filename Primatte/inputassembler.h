#pragma once
#include <cstring>
#include <vector>
#include "inputcleanup.h"
#include <opencv2/core/core.hpp>
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
            enum TargetColourspace {ETCS_RGB, ETCS_HSV, ETCS_LAB};

            /** The colour space to which the input will be converted. */
            TargetColourspace targetColourspace;

            /** Set the image source to be copied from.
                The source mat is converted into floating point rgb values.
                8-bit, 16-bit and floating point formats are supported.
                Expects a 3-component image. */
            const cv::Mat* source;

            InputCleanupDescriptor ipd;

            /** A normalised background point, in the colourspace of the source image.
              * Example, rgb(0,0,1), rgb(0.2,0.4,0) */
            math::vec3 backgroundPoint;

            InputAssemblerDescriptor()
            {
                memset(this, 0, sizeof(*this));
            }
        };

        /** The input assembler class. */
        class InputAssembler
        {
            cv::Mat mMatF;
            std::vector<math::vec3> mPoints;
            math::vec3 mBackground;
            InputAssemblerDescriptor::TargetColourspace mColourSpace;
        public:

            /** Initialises the input, throwing an exception if failed. */
            InputAssembler(InputAssemblerDescriptor& desc);

            /** Returns the internal points. */
            const std::vector<math::vec3>& points() const;

            /** Returns the internal floating point image. */
            const cv::Mat& mat() const;

            /** Returns the background point in the correct colour space. */
            math::vec3 background() const;

            //Converts from internal colour space to rgb
            cv::Point3f debugGetPointColour(math::vec3 p) const;

            /** Loads a cv::Mat from an image file in rgb format. */
            static cv::Mat loadMatFromFile(const char *path);


            /** Converts the given RGB pixels to hsv. */
            static void rgbToHsv(cv::Mat* rgb);

            /** Helper function */
            static cv::Mat loadRgbMatFromFile(const char* path);

        };


    }
}
