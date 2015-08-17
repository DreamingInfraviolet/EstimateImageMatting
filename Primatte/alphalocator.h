#pragma once
#include "ialphalocator.h"

/** Implements the IAlphaLocator class based in simple distance from the points. */
namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            class AlphaDistanceLocator : public IAlphaLocator
            {
            public:
                virtual std::vector<float> findAlphas(
                        const BoundingPolyhedron* polyhedrons,
                        const size_t polyhedroncount,
                        const std::vector<Point>& points,
                        const Point reference) const;
            };
        }
    }
}
