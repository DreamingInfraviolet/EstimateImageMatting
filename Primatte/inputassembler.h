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
            enum TargetColourspace {ETCS_RGB, ETCS_HSV, ETCS_LAB};

            /** The colour space to which the input will be converted. */
            TargetColourspace targetColourspace;

            /** The source from which the input should be copied. */
            cv::Mat* source;

            InputProcessingDescriptor ipd;

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
            bool* mGrid; //A 3D grid
            size_t mGridSize;
            InputAssemblerDescriptor::TargetColourspace mColourSpace;
        public:

            /** Initialises the input, throwing an exception if failed. */
            InputAssembler(InputAssemblerDescriptor& desc);

            ~InputAssembler();

            /** Returns the internal points. */
            const std::vector<math::vec3>& points() const;

            /** Returns the internal floating point image. */
            const cv::Mat& mat() const;

            /** Returns the background point in the correct colour space. */
            math::vec3 background() const;

            const bool* grid() const {return mGrid;}

            size_t gridSize() const { return mGridSize; }

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
