#pragma once
#include <vector>
#include "cgal.h"

namespace anima
{
    namespace alg
    {
        /** An abstract class that defines the basic functionality required by an algorithm.
          * Deviates towards an abstract class by implementing basic functionality for convenience. */
        class IAlgorithm
        {
        protected:
            /* Must be set by prepare() when prepared. */
            bool mPrepared;
        public:

            /** Trivial constructor. */
            IAlgorithm() : mPrepared(false) {}

            /** Trivial virtual destructor */
            virtual ~IAlgorithm(){}

            /** Prepares alpha computation for the given data set. Needed for debug draw.
              * Example activity might be collapsing polyhedrons around the input points.
                Must set mPrepared=when done. */
            virtual void prepare() = 0;

            /** Computes the alpha for the given set of points.*/
            virtual std::vector<float> computeAlphas(const std::vector<Point>& input) = 0;

            /** Indicates whether prepare() has been called. */
            virtual bool prepared()
            {
                return mPrepared;
            }

            /** Draws a 3D representation of the algorithm. */
            virtual void debugDraw() = 0;
        };
    }
}
