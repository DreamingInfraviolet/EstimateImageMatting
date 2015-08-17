#pragma once
#include "IAlgorithm.h"
#include "pixel.h"
#include <opencv2/opencv.hpp>

/** The output assembler is responsible for evaluating an algorithm
  * and applying it to an image, as well as saving the result. */
namespace anima
{
    namespace oa
    {
        /**
          * An object that uses an algorithm to create output accoring to the
          * inputed parameters.
          * */

        struct OutputAssemblerDescriptor
        {
            alg::IAlgorithm* algorithm;

            /** The image to generate alpha for. */
            cv::Mat* imageSource;

            /** Whether to save the result to disk. */
            bool generateFile;

            /** Whether to show a window with the resulting image. */
            bool showPreview;

            struct
            {
                /** Whether to save the image as a floating point bmp file. */
                bool saveFloat;
                /** Floating point image location - must be bmp. */
                const char* floatPath;
                /** Monochrome rgb image location. Must be cv image type. */
                const char* normalPath;
            } fileOptions;

            struct
            {
                /** The destination memory for storing raw alpha floats. Ignores if null.*/
                byte* destinationRaw;

                /** The size in bytes of the raw destination memory. */
                size_t destinationRawSize;

                /** The destination image to write alpha to. Ignores if null. */
                cv::Mat* destinationImage;
            } memoryOptions;

            struct
            {
                const char* windowName;
            } previewOptions;

            OutputAssemblerDescriptor()
            {
                memset(this, 0, sizeof(*this));
            }
        };

        class OutputAssembler
        {
        public:
            void assemble(OutputAssemblerDescriptor desc);

            static size_t requiredDestinationSize();
        };
    }
}
