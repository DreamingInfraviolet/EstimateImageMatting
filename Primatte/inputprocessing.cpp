#include "inputprocessing.h"
#include <vector>
#include <set>
#include <stdexcept>
#include <random>
#include <functional>
#include "matrixd.h"
#include "io.h"
namespace anima
{
    namespace ia
    {
        std::vector<math::vec3> RemoveDuplicatesWithGrid(const cv::Mat& mat, unsigned gridSize)
        {
            START_TIMER(CleaningWithGrid);

            const unsigned r = mat.rows, c = mat.cols;

            std::vector<math::vec3> points;
            points.reserve(r*c/50);

            const unsigned gridCubed = gridSize*gridSize*gridSize;
            const unsigned gridSizeMinusOne = gridSize-1;

            bool* grid = new(std::nothrow) bool[gridCubed]();

            if(!grid)
                throw std::runtime_error("Unable to allocate 3D grid for input processing");

            for (unsigned i = 0; i < r; ++i)
                for(unsigned j = 0; j < c; ++j)
                {
                    cv::Point3f p = mat.at<cv::Point3f>(i,j);
                    cv::Point3i pi = p*(int)gridSize;
                    if(unsigned(pi.x) >= gridSize)
                        pi.x = gridSizeMinusOne;
                    if(unsigned(pi.y) >= gridSize)
                        pi.y = gridSizeMinusOne;
                    if(unsigned(pi.z) >= gridSize)
                        pi.z = gridSizeMinusOne;

                    bool& b = grid[pi.x + gridSize*(pi.y + gridSize*pi.z)];
                    if (!b)
                    {
                        b = true;
                        points.push_back(math::vec3(p.x, p.y, p.z));
                    }
                }

            delete[] grid;

            END_TIMER(CleaningWithGrid);

            return points;
        }

        void RandomSimplify(std::vector<math::vec3>* points, float percentageToRemove)
        {
            START_TIMER(RandomSimplifying);
            size_t initialSize = points->size();

            std::random_device rd;
            std::mt19937 random(rd());
            std::shuffle(points->begin(), points->end(), random);

            points->erase(points->begin()+(int)(points->size()*(1.f-percentageToRemove/100.f)), points->end());

            Inform("" + ToString(points->size()/float(initialSize)*100) + "% of points remain (" +
                   ToString(points->size()) + "/" + ToString(initialSize)+")");

            END_TIMER(RandomSimplifying);
        }


        std::vector<math::vec3> ProcessPoints(const cv::Mat& mat, InputProcessingDescriptor desc)
        {
            assert(mat.type() == CV_32FC3);

            if(!desc.validate())
                throw std::runtime_error("Could not validate input processor.");
            if(mat.rows*mat.cols==0 || mat.data==nullptr)
                throw std::runtime_error("Invalid mat");

            auto points = RemoveDuplicatesWithGrid(mat, desc.gridSize);

            if(desc.randomSimplify)
                RandomSimplify(&points, desc.randomSimplifyPercentage);
            return points;
        }
    }
}
