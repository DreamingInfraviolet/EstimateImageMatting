#pragma once
#include "matrixd.h"
#include "spherepolyhedron.h"

/** A polyhedron object that's capable of wrapping itself around a set of points. */
namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            class IFittingAlgorithm;

            /** The descriptor used to create the polyhedron. */
            struct BoundingPolyhedronDescriptor
            {
                /* Number of faces around the up axis to have. Must be > 3 */
                int phiFaces;

                /* The number of horizontal faces to have. Must be > 2 */
                int thetaFaces;

                /* The polyhedron is scaled by this factor after being positioned. */
                float scaleMultiplier;

                /* The fitting algorithm to use to fit the polyhedron around the points. */
                IFittingAlgorithm* fitter;
            };

            class BoundingPolyhedron : public SpherePolyhedron
            {
            private:
                /* The descriptor object. */
                BoundingPolyhedronDescriptor mDesc;

                /* Indicates whether the polyhedron is initialised.
                 * Only initialised polyhedrons may be used after creation. */
                bool mInitialised;

            public:
                /** Constructs an uninitialised object. */
                BoundingPolyhedron() : mInitialised(false) {}

                /** Constructs the object from the descriptor. */
                BoundingPolyhedron(BoundingPolyhedronDescriptor desc, const math::vec3 centre);

                /** Positions the polyhedral sphere around the points,
                  * only using linear transformations.
                  * @param points The points around which to position the
                                  polyhedron.
                  */
                void positionAround(const std::vector<math::vec3>& points);


                void shrink(const std::vector<math::vec3>& points);

                void expand(const std::vector<math::vec3>& points);
            };
        }
    }
}
