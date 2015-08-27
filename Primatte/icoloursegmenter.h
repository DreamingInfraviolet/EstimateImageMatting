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
            std::vector<math::vec3> outer;
            std::vector<math::vec3> inner;
        };


            class IColourSegmenter
            {
            public:
                /** Computes the subset.
                  * @param points The points the subset of which is to be found.
                  * @param background The point around which to find the subset.
                  * @param approximateRadius Loosely-defined cut-off point.
                  */
                virtual SegmenterResult segment(const std::vector<math::vec3>& points,
                                                   const math::vec3 background,
                                                   float approximateRadius) const = 0;
            };

//            class IColourSegmenter
//            {
//            public:
//                /** Computes the subset, returning <outer,inner>
//                  * @param points The points the subset of which is to be found.
//                  * @param background The point around which to find the subset.
//                  * @param approximateRadius Loosely-defined cut-off point.
//                  */
//                virtual std::pair<std::vector<math::vec3>,std::vector<math::vec3> >
//                                   segment(const std::vector<math::vec3>& points,
//                                                   const math::vec3 background,
//                                                   float approximateRadius) const = 0;
//            };
        }
    }
}
