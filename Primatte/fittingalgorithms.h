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
            class Test2Fitting : public IFittingAlgorithm
            {
            public:
                virtual void shrink(BoundingPolyhedron &poly, const std::vector<math::vec3>& points) const;
                virtual void expand(BoundingPolyhedron& poly, const std::vector<math::vec3>& points) const;
            }; //End of class

            /** Does nothing */
            class NoFitting : public IFittingAlgorithm
            {
            public:
                virtual void shrink(BoundingPolyhedron& poly, const std::vector<math::vec3>& points) const;
                virtual void expand(BoundingPolyhedron& poly, const std::vector<math::vec3>& points) const;
            }; //End of class
        }
    }
}
