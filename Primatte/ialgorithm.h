#pragma once
#include <vector>
#include <opencv2/core/core.hpp>
#include "inputassembler.h"

namespace anima
{
    namespace alg
    {
        /** An abstract class that defines the basic functionality required by an algorithm.
          * Deviates towards an abstract class by implementing basic functionality for convenience. */
        class IAlgorithm
        {
        protected:

            const ia::InputAssembler* mInput;
        public:

            /** Trivial virtual destructor */
            virtual ~IAlgorithm() {}

            /** Sets the current input. */
            virtual void setInput(const ia::InputAssembler* input)
            {
                mInput = input;
            }

            /** Prepares alpha computation for the given data set. Needed for debug draw.
              * Example activity might be collapsing polyhedrons around the input points.
                Must set mPrepared=when done. */
            virtual void analyse() = 0;

            /** Computes the alpha for the internal input.*/
            virtual cv::Mat computeAlphas() const = 0;

            /** Draws a 3D representation of the algorithm. */
            virtual void debugDraw() const = 0;
        };
    }
}
