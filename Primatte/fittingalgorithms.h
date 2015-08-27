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
            /** Use exact fitting by trying to move points inside/outside while possible. */
            class StableFitting : public IFittingAlgorithm
            {
                int mNoOfIterations;

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
                virtual void shrink(BoundingPolyhedron& poly, const std::vector<math::vec3>& points, math::vec3 backgroundPoint,  float minimumDistance) const {}
                virtual void expand(BoundingPolyhedron& poly, const std::vector<math::vec3>& points, IColourSegmenter* segmenter, math::vec3 backgroundPoint, float startingRadius, float maximumRadius) const {}
            }; //End of class
        }
    }
}
