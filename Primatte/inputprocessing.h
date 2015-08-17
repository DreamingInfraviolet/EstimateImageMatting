#pragma once
#include <vector>
#include <set>
#include "pixel.h"
#include "cgal.h"

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

            /** This controls the bit precision per component for comparison
            * when removing duplicates. Range from 0 to 8 (lower=better).
            */
            size_t comparisonBitsToIgnore;

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

            bool validate()
            {
                if(comparisonBitsToIgnore > 8 ||
                    removeOutliersPercentage<0 ||
                        gridSimplifyEpsilon < 0 ||
                        randomSimplifyPercentage < 0)
                    return false;
                else
                    return true;
            }
        };

        /** Takes in a vector of points and a descriptor and returns the cleaned up corresponding normalised points. */
        std::vector<alg::Point> ProcessPixels(const std::vector<PixelRGB8>& pixels, InputProcessingDescriptor desc);

        namespace detail
        {
            /** Finds list of unique pixels. */
            std::set<PixelRGB8> RemoveDuplicatePixels(const std::vector<PixelRGB8>& pixels, int bitsToIgnore);

            /** Converts pixels to points. */
            std::vector<alg::Point> PixelsToPoints(const std::set<PixelRGB8>& pixels);

            /** Cleans up the point data accoring to the descriptor. */
            void CleanPointData(std::vector<alg::Point>* points, InputProcessingDescriptor desc);
        }
    }
}
