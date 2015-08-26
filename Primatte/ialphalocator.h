#pragma once
#include "boundingpolyhedron.h"
#include <vector>
#include <opencv2/core/core.hpp>
#include "inputassembler.h"

/**
  * Given n polyhedrons, a background centre point and a list of points,
  * return a vector of alphas corresponding to the list of points
  * in relation to the background point.
  */

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            class IAlphaLocator
            {
            public:
                /** Calculates the alpha for a set of points.
                  * @param polyhedrons The polyhedrons usd by primatte
                                       ordered from inner to outer.
                  * @param polyhedronCount The number of polyhedrons.
                  * @param input The initialised input structure.
                  */
                virtual cv::Mat findAlphas(
                        const BoundingPolyhedron* polyhedrons,
                        const size_t polyhedronCount,
                        const ia::InputAssembler &input) const = 0;
            };
        }
    }
}
