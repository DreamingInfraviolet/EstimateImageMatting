#pragma once
#include <opencv2/core/core.hpp>
#include "matrixd.h"
/** Given an F32 mat of background colours, this class must calculate
  * the most dominant colour and return it.
  */

namespace anima
{
    namespace ia
    {
        class IAverageBackgroundColourLocator
        {
        public:
            virtual ~IAverageBackgroundColourLocator(){}
            virtual math::vec3 findColour(const cv::Mat& mat) const = 0;
        };
    }
}
