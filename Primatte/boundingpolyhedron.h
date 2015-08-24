#pragma once
#include "matrixd.h"
#include "ifittingalgorithm.h"

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
                /* The path of the .obj mesh to use. */
                const char* meshPath;

                /* The polyhedron is scaled by this factor after being positioned. */
                Real scaleMultiplier;

                /* The fitting algorithm to use to fit the polyhedron around the points. */
                IFittingAlgorithm* fitter;

                /* The desired centre of the polyhedron. */
                Point centre;
            };

            class BoundingPolyhedron
            {
            private:
                /* The internal polyhedron object. */
                Polyhedron mPoly;

                /* The initial radius of the polyhedron. */
                Real mInitialRadius;

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
                void positionAround(const std::vector<Point>& points);

                /** Uses the supplied fitting algorithm to wrap the polyhedron
                  * around the points using per-vertex operations.
                  * @param points The points around which the polyhedron should be fitted.
                  * @param centre The point towards which the polyhedron should be fitted.
                  */
                void fit(const std::vector<Point>& points, Point centre);

                /** Draws a preview of the polyhedron with the given colour.
                  * @param colour A colour with components in the range [0,255]
                  */
                void debugDraw(math::vec3i colour) const;

                /** Returns the initial radius of the sphere. */
                Real initialRadius() const;

                /** Returns the initial centre of the sphere. */
                Point centre() const { return mDesc.centre; }

                const Polyhedron& polyhedron() const { return mPoly; }
            };
        }
    }
}
