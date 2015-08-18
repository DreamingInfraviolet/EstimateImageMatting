#pragma once
#include "icoloursegmenter.h"

/* A series of classes implementing the IColourSegmenter interface. */

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            /** This class segments colours according to distance,
              * cutting off the points that are too far away. */
            class DistanceColourSegmenter : public IColourSegmenter
            {
            public:
                virtual std::vector<Point> segment(const std::vector<Point>& points,
                                                   const Point reference,
                                                   float approximateRadius) const;
            };
        }
    }
}
