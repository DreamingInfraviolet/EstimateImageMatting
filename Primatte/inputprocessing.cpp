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
        std::pair<std::vector<math::vec3>, bool*> RemoveDuplicatesWithGrid(const cv::Mat& mat, unsigned gridSize)
        {
            Inform("Cleaning point data using 3D grid...");

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

            return std::make_pair(points,grid);
        }

        void RandomSimplify(std::vector<math::vec3>* points, float percentageToRemove)
        {
            Inform("Removing points randomly...");
            size_t initialSize = points->size();

            std::random_device rd;
            std::mt19937 random(rd());
            std::shuffle(points->begin(), points->end(), random);

            points->erase(points->begin()+(int)(points->size()*(1.f-percentageToRemove/100.f)), points->end());

            Inform("" + ToString(points->size()/float(initialSize)*100) + "% of points remain (" +
                   ToString(points->size()) + "/" + ToString(initialSize)+")");
        }


        std::pair<std::vector<math::vec3>, bool*> ProcessPoints(const cv::Mat& mat, InputProcessingDescriptor desc)
        {
            assert(mat.type() == CV_32FC3);

            if(!desc.validate())
                throw std::runtime_error("Could not validate input processor.");
            if(mat.rows*mat.cols==0 || mat.data==nullptr)
                throw std::runtime_error("Invalid mat");

            auto pointsAndGrid = RemoveDuplicatesWithGrid(mat, desc.gridSize);

            if(desc.randomSimplify)
                RandomSimplify(&pointsAndGrid.first, desc.randomSimplifyPercentage);
            return pointsAndGrid;
        }
    }
}
