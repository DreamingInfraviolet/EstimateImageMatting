#pragma once
#include "matrixd.h"
#include "ifittingalgorithm.h"
#include "spherepolyhedron.h"

/** A polyhedron object that's capable of wrapping itself around a set of points. */
namespace anima
{
    namespace alg
    {
        namespace primatte
        {
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

                /* The desired centre of the polyhedron. */
                math::vec3 centre;
            };

            class BoundingPolyhedron
            {
            private:
                /* The internal polyhedron object. */
                SpherePolyhedron mPoly;

                /* The initial radius of the polyhedron. */
                float mInitialRadius;

                /* The descriptor object. */
                BoundingPolyhedronDescriptor mDesc;

                /* Indicates whether the polyhedron is initialised.
                 * Only initialised polyhedrons may be used after creation. */
                bool mInitialised;

            public:
                /** Constructs an uninitialised object. */
                BoundingPolyhedron() : mInitialRadius(0),mInitialised(false) {}

                /** Constructs the object from the descriptor. */
                BoundingPolyhedron(BoundingPolyhedronDescriptor desc);

                /** Positions the polyhedral sphere around the points,
                  * only using linear transformations.
                  * @param points The points around which to position the
                                  polyhedron.
                  */
                void positionAround(const std::vector<math::vec3>& points);

                /** Uses the supplied fitting algorithm to wrap the polyhedron
                  * around the points using per-vertex operations.
                  * @param points The points around which the polyhedron should be fitted.
                  * @param centre The point towards which the polyhedron should be fitted.
                  */
                void fit(const std::vector<math::vec3>& points, math::vec3 centre);

                /** Draws a preview of the polyhedron with the given colour.
                  * @param colour A colour with components in the range [0,255]
                  */
                void debugDraw(math::vec3 colour) const;

                /** Returns the initial radius of the sphere. */
                float initialRadius() const;

                /** Returns the initial centre of the sphere. */
                math::vec3 centre() const { return mDesc.centre; }

                const SpherePolyhedron& polyhedron() const { return mPoly; }
            };
        }
    }
}
