#pragma once
#include "spherepolyhedron.h"
#include "matrixd.h"

/** An interface defining the functionality needed by an algorithm that
  * fits a polyhedron (that's assumed to be around the points) tightly
  * on per-vertex granularity (aka, shrinking the sphere).
  */
namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            class IFittingAlgorithm
            {
            public:
                virtual ~IFittingAlgorithm(){}

                /** Fits the polyhedron around the points.
                  * @param poly The polyhedron to be shrunk.
                  * @param points The points around which to shrink.
                  */
                virtual void shrink(SpherePolyhedron& poly, const std::vector<math::vec3>& points) const = 0;

                /** Fits the polyhedron around the points.
                  * @param poly The polyhedron to be expanded.
                  * @param points The points inside which to expand.
                  */
                virtual void expand(SpherePolyhedron& poly, const std::vector<math::vec3>& points) const = 0;
            };
        }
    }
}
