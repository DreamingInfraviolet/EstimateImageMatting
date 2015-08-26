#pragma once
#include "iaveragebackgroundcolourlocator.h"

namespace anima
{
    namespace ia
    {
        class ABCL_BarycentreBased : public IAverageBackgroundColourLocator
        {
        public:
            virtual math::vec3 findColour(const cv::Mat& mat) const;

        };
    }
}
