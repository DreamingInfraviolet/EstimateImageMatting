#pragma once
#include "iaveragebackgroundcolourlocator.h"

namespace anima
{
    namespace ia
    {
    /** Chooses the average background colour as the barycentre of all points. */
        class ABCL_BarycentreBased : public IAverageBackgroundColourLocator
        {
        public:
            virtual math::vec3 findColour(const cv::Mat& mat) const;

        };
    }
}
