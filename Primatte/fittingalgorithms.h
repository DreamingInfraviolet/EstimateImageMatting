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
                virtual void fit(SpherePolyhedron& poly,
                                 const math::vec3 centre,
                                 const std::vector<math::vec3>& points) const;
            }; //End of class

            /** Does nothing */
            class NoFitting : public IFittingAlgorithm
            {
            public:
                virtual void fit(SpherePolyhedron& poly,
                                 const math::vec3 centre,
                                 const std::vector<math::vec3>& points) const;
            }; //End of class
        }
    }
}
