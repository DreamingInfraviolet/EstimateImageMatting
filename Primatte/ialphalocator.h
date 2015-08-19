#pragma once
#include "boundingpolyhedron.h"
#include "cgal.h"
#include <vector>
#include <opencv2/opencv.hpp>
/**
  * Given n polyhedrons, a reference centre point and a list of points,
  * return a vector of alphas corresponding to the list of points
  * in relation to the reference point.
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
                  * @param matIn The mat for which alpha must be generated.
                  * @param reference The background colour point.
                  */
                virtual cv::Mat findAlphas(
                        const BoundingPolyhedron* polyhedrons,
                        const size_t polyhedronCount,
                        const cv::Mat& matIn,
                        const Point reference) const = 0;
            };
        }
    }
}
