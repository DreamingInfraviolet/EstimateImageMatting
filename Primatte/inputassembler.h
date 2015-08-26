#pragma once
#include <cstring>
#include <vector>
#include <opencv2/core/core.hpp>
#include "matrixd.h"

    /**
      * This class is responsible for taking an InputAssemblerDescriptor object,
      * and load+store the corresponding data.
      * */

namespace anima
{
    namespace ia
    {
        class IAverageBackgroundColourLocator;

        /** The descriptor of the input data/source. */
        struct InputAssemblerDescriptor
        {
            enum TargetColourspace {ETCS_RGB, ETCS_HSV, ETCS_LAB};

            /** The colour space to which the input will be converted. */
            TargetColourspace targetColourspace;

            IAverageBackgroundColourLocator* backgroundLocator;

            /** Set the image source to be copied from.
                The source mat is converted into floating point rgb values.
                8-bit, 16-bit and floating point formats are supported.
                Expects a 3-component image. */
            const cv::Mat* foregroundSource;

            const cv::Mat* backgroundSource;

            /** The input processing descriptor, setting out pixel cleaning options. */
            struct InputCleanupDescriptor
            {
                /** Wheter to remove points at random. */
                bool randomSimplify;

                /** Percentage of points to remove randomly. */
                float randomSimplifyPercentage;

                /** Used for early stage pre-processing where only one point is kept per 3D grid box. */
                unsigned gridSize;

                bool validate()
                {
                    if(randomSimplifyPercentage < 0 ||
                       randomSimplifyPercentage > 100)
                        return false;
                    else
                        return true;
                }
            } ipd;

            InputAssemblerDescriptor()
            {
                memset(this, 0, sizeof(*this));
            }
        };

        /** The input assembler class. */
        class InputAssembler
        {
            cv::Mat mForegroundF, mBackgroundF;
            std::vector<math::vec3> mPoints, mBackgroundPoints;
            math::vec3 mBackground;
            InputAssemblerDescriptor::TargetColourspace mColourSpace;
        public:

            /** Initialises the input, throwing an exception if failed. */
            InputAssembler(InputAssemblerDescriptor& desc);

            /** Returns the internal points. */
            const std::vector<math::vec3>& points() const;

            const std::vector<math::vec3>& backgroundPoints() const
            {
                return mBackgroundPoints;
            }

            /** Returns the internal floating point image. */
            const cv::Mat& mat() const;

            /** Returns the most dominant background point in the correct colour space. */
            math::vec3 background() const;

            //Converts from internal colour space to rgb
            cv::Point3f debugGetPointColour(math::vec3 p) const;

            /** Loads a cv::Mat from an image file in rgb format. */
            static cv::Mat loadMatFromFile(const char *path);


            /** Converts the given RGB pixels to hsv. */
            static void rgbToHsv(cv::Mat* rgb);

        };


    }
}
