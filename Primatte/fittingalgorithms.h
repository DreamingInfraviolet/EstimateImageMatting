#pragma once
#include "ifittingalgorithm.h"

/*
  * Various polyhedron fitting algorithms implementing the IFittingAlgorithm interface.
  */
namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            /** Use exact fitting by trying to move points inside/outside while possible.
                Points are guaranteed to not go through the polyhedron.
                An alternative to try out might be to allow for a certain number of points to be ignored,
                reducing outlier effect. */
            class StableFitting : public IFittingAlgorithm
            {
                //Number of iterations to perform.
                int mNoOfIterations;

                /** Counts the number of points from the points vector that are inside the bounding polyhedron. */
                static unsigned countPointsInside(const std::vector<math::vec3>& points, BoundingPolyhedron& poly);

            public:

                StableFitting(int numberOfIterations) : mNoOfIterations(numberOfIterations){}

                virtual void shrink(BoundingPolyhedron& poly, const std::vector<math::vec3>& points, math::vec3 backgroundPoint,  float minimumDistance) const;
                virtual void expand(BoundingPolyhedron& poly, const std::vector<math::vec3>& points, IColourSegmenter* segmenter, math::vec3 backgroundPoint, float startingRadius, float maximumRadius) const;
            }; //End of class

            /** Does nothing */
            class NoFitting : public IFittingAlgorithm
            {
            public:
                virtual void shrink(BoundingPolyhedron&, const std::vector<math::vec3>&, math::vec3,  float) const {}
                virtual void expand(BoundingPolyhedron&, const std::vector<math::vec3>&, IColourSegmenter*, math::vec3, float, float) const {}
            }; //End of class
        }
    }
}
