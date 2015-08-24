#pragma once
#include "ialphalocator.h"

/** Implements the IAlphaLocator class based in simple distance from the points. */
namespace anima
{
    namespace alg
    {
        namespace primatte
        {
        /** Sends out rays to determine the alpha. */
        class AlphaRayLocator : public IAlphaLocator
        {
        public:
            virtual cv::Mat findAlphas(
                    const BoundingPolyhedron* polyhedrons,
                    const size_t polyhedronCount,
                    const ia::InputAssembler &input) const;
        };
        }
    }
}
