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
        /** Uses a dumb algorithm based on the distance to the nearest point.
          * Doesn't yield good results, but ok for preliminary testing. */
        class TestVertexFitting : public IFittingAlgorithm
        {
        public:
            virtual void fit(Polyhedron& poly,
                             const Point centre,
                             const std::vector<Point>& points) const;
        }; //End of class

        /** Trying...*/
        class Test2Fitting : public IFittingAlgorithm
        {
        public:
            virtual void fit(Polyhedron& poly,
                             const Point centre,
                             const std::vector<Point>& points) const;
        }; //End of class
        }
    }
}
