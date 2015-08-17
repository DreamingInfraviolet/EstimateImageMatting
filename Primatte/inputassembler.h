#pragma once
#include <cstring>
#include <vector>
#include "pixel.h"
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
            enum InputSource {EMEMORY, EFILE};
            enum PixelType {EPIXEL_RGB8};

            /** The source from which the input should be copied. */
            InputSource inputSource;

            union
            {
                struct
                {

                    /** The pixel type. Currently only RGB8 is supported. */
                    PixelType type;

                    /** The pixel data. */
                    const byte* data;

                    /** The maximum size of the data in bytes. */
                    size_t dataSize;

                    /** The size allocated to each pixel (sizeof(pixel)). */
                    size_t step;
                } inputSourceMemory;

                struct
                {
                    /** The file to load. */
                    const char* path;
                } inputSourceFile;
            };

            InputProcessingDescriptor ipd;

            InputAssemblerDescriptor()
            {
                memset(this, 0, sizeof(*this));
            }
        };

        /** The input assembler class. */
        class InputAssembler
        {
            std::vector<PixelRGB8> mPixels;
            std::vector<alg::Point> mPoints;
        public:

            /** Initialises the input, throwing an exception if failed. */
            InputAssembler(InputAssemblerDescriptor& desc);

            /** Returns the internal pixels. */
            const std::vector<PixelRGB8>& pixels() const;

            /** Returns the internal points. */
            const std::vector<alg::Point>& points() const;

            /** Loads a vector of pixels from an image file. */
            static std::vector<PixelRGB8> loadFromFile(const char* path);

            /** Loads a cv::Mat from an image file in rgb format. */
            static cv::Mat loadRgbMatFromFile(const char *path);
        };


    }
}
