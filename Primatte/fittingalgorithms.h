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
            /** Trying...*/
            class Test2Fitting : public IFittingAlgorithm
            {
            public:
                virtual void shrink(SpherePolyhedron& poly, const std::vector<math::vec3>& points) const;
                virtual void expand(SpherePolyhedron& poly, const std::vector<math::vec3>& points) const;
            }; //End of class

            /** Does nothing */
            class NoFitting : public IFittingAlgorithm
            {
            public:
                virtual void shrink(SpherePolyhedron& poly, const std::vector<math::vec3>& points) const;
                virtual void expand(SpherePolyhedron& poly, const std::vector<math::vec3>& points) const;
            }; //End of class
        }
    }
}
