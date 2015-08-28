#pragma once
#include "ialphalocator.h"

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
        /** Sends out rays to determine the alpha, interpolating between the two
          * spheres. It is done so that:
          * If outside outer sphere, alpha = 1
          * If inside inner sphere, alpha = 0
          * If in the middle, interpolate.
          * It expects that the bounding polyhedrons are sorted in
          * increasing order so that any ray sent is guaranteed that the inner
          * intersection is closer than the outer.
          * This particular algorithm requires at least two polyhedrons.
        * */
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
