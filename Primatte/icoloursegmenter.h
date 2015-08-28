#pragma once
#include "matrixd.h"
#include <vector>
/**
  * Given a vector of points, this class must return a subset of those points.
  * based on their distance from a background point.
  * It essentially choose the points against which the polyhedrons must be fitted.
  */

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
        struct SegmenterResult
        {
            //The points chosen to be outside.
            std::vector<math::vec3> outer;

            //The points chosen to be inside.
            std::vector<math::vec3> inner;

            //The union of these two constitutes the input points to the segmenter.
        };


        class IColourSegmenter
        {
        public:

            virtual ~IColourSegmenter(){}

            /** Computes the subset.
              * @param points The points the subset of which is to be found.
              * @param background The point around which to find the subset.
              * @param approximateRadius Loosely-defined cut-off point.
              */
            virtual SegmenterResult segment(const std::vector<math::vec3>& points,
                                               const math::vec3 background,
                                               float approximateRadius) const = 0;
        };
        }
    }
}
