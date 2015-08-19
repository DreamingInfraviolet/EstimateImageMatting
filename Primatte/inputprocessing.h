#pragma once
#include <vector>
#include <set>
#include "cgal.h"
#include <opencv2/opencv.hpp>

/* A series of functions that convert raw RGB8 pixels into a vector of
 * cleaned up Points. */

namespace anima
{
    namespace ia
    {
        /** The input processing descriptor, setting out pixel cleaning options. */
        struct InputProcessingDescriptor
        {
            enum SimplifyOrder {EOutliersGridRandom, EOutliersRandomGrid,
                                EGridOutliersRandom, EGridRandomOutliers,
                                ERandomGridOutliers, ERandomOutliersGrid};

            /** Epsilon for grid cell removal*/
            double gridSimplifyEpsilon;

            /** Percentage of points to remove randomly. */
            double randomSimplifyPercentage;

            /** Percentage of points to remove. */
            double removeOutliersPercentage;

            /** number of neighbors.*/
            size_t removeOutliersK;

            /** The order in which the cleanup operations should be performed.
              * If a cleanup operation is not selected, it will be ignored.
              * Cleanup is run after duplicate removal.
              */
            SimplifyOrder order;

            /** Whether to remove outliers. */
            bool removeOutliers;
            /** Merges points which belong to the same cell of a grid of cell size = epsilon. */
            bool gridSimplify;
            /** Removes points at random. */
            bool randomSimplify;

            /** Used for early stage pre-processing where only one point is kept per 3D grid box. */
            unsigned gridSize;

            bool validate()
            {
                if(
                    removeOutliersPercentage<0 ||
                        gridSimplifyEpsilon < 0 ||
                        randomSimplifyPercentage < 0 ||
                        gridSize > 255)
                    return false;
                else
                    return true;
            }
        };

        /** Takes in a vector of points and a descriptor and returns the cleaned up corresponding normalised points. */
        std::vector<alg::Point> ProcessPoints(const cv::Mat& mat, InputProcessingDescriptor desc);

        namespace detail
        {
            /** Constructs a 3D grid of booleans to ensure that only one point is kept per box.
              * Memory consumption = sizeof(bool)*gridSize*gridSize*gridSize. Fairly fast.
              * Also converts to a vector of points.
              */
            std::vector<alg::Point> RemoveDuplicatesWithGrid(const cv::Mat& mat, unsigned gridSize);

            /** Cleans up the point data accoring to the descriptor using CGAL. */
            void CleanPointData(std::vector<alg::Point>* points, InputProcessingDescriptor desc);
        }
    }
}

