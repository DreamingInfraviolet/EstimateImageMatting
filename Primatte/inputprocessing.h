#pragma once
#include <vector>
#include <set>
#include "matrixd.h"
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
            /** Percentage of points to remove randomly. */
            float randomSimplifyPercentage;

            /** Used for early stage pre-processing where only one point is kept per 3D grid box. */
            unsigned gridSize;

            /** Removes points at random. */
            bool randomSimplify;


            bool validate()
            {
                if(randomSimplifyPercentage < 0 ||
                   randomSimplifyPercentage > 100)
                    return false;
                else
                    return true;
            }
        };

        /** Takes in a vector of points and a descriptor and returns the cleaned up corresponding normalised points. #
          * also returns a pointer to the 3d grid. Memory management up to the user.*/
        std::vector<math::vec3> ProcessPoints(const cv::Mat& mat, InputProcessingDescriptor desc);

        /** Constructs a 3D grid of booleans to ensure that only one point is kept per box.
          * Memory consumption = sizeof(bool)*gridSize*gridSize*gridSize. Fairly fast.*/
        std::vector<math::vec3> RemoveDuplicatesWithGrid(const cv::Mat& mat, unsigned gridSize);

        /** Randomly deletes the points. */
        void RandomSimplify(std::vector<math::vec3>* points, float percentageToRemove);

    }
}
