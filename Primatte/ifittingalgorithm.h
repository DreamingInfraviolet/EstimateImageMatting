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
                  * @param poly The polyhedron reference to be fitted.
                  * @param centre The centre towards which the polyhedron vertices should be moved.
                  * @param points The points around which to fit.
                  */
                virtual void fit(SpherePolyhedron& poly, const math::vec3 centre, const std::vector<math::vec3>& points) const = 0;
            };
        }
    }
}
