#pragma once
#include "matrixd.h"
#include <vector>
/**
  * Given a vector of points, this class must return a subset of those points.
  * based on their distance from a reference point and the given data parameters.
  */

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            class IColourSegmenter
            {
            public:
                /** Computes the subset.
                  * @param points The points the subset of which is to be found.
                  * @param reference The point around which to find the subset.
                  * @param approximateRadius Loosely-defined cut-off point.
                  */
                virtual std::vector<math::vec3> segment(const std::vector<math::vec3>& points,
                                                   const math::vec3 reference,
                                                   float approximateRadius) const = 0;
            };
        }
    }
}
