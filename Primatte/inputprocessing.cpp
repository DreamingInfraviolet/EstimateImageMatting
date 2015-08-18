#include "inputprocessing.h"
#include <vector>
#include <set>
#include "cgal.h"
#include <CGAL/remove_outliers.h>
#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/random_simplify_point_set.h>
#include "matrixd.h"
#include "io.h"

namespace anima
{
    namespace ia
    {
        namespace detail
        {


        std::vector<alg::Point> RemoveDuplicatesWithGrid(const cv::Mat& mat, unsigned gridSize)
        {
            Inform("Cleaning point data using 3D grid...");

            const unsigned r = mat.rows, c = mat.cols;

            std::vector<alg::Point> points;
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
                        points.push_back(alg::Point(p.x, p.y, p.z));
                    }
                }

            Inform("" + ToString(points.size()/float(r*c)*100) + "% of points remain (" +
                   ToString(points.size()) + "/" + ToString(r*c)+")");
            delete [] grid;
            return points;
        }

            void CleanPointData(std::vector<alg::Point>* points, InputProcessingDescriptor desc)
            {
                Inform("Cleaning point data using CGAL...");
                size_t initialSize = points->size();

                #define CLEAN_OUTLIERS {if(desc.removeOutliers) \
                                        { \
                                            points->erase(CGAL::remove_outliers \
                                                          (points->begin(), points->end(),desc.removeOutliersK, \
                                                           desc.removeOutliersPercentage), \
                                                points->end());}}
                #define CLEAN_GRID {if(desc.gridSimplify) \
                                    { \
                                        points->erase(CGAL::grid_simplify_point_set \
                                                      (points->begin(), points->end(), desc.gridSimplifyEpsilon), \
                                            points->end());}}
                #define CLEAN_RANDOM {if(desc.randomSimplify) \
                                      { \
                                          points->erase(CGAL::random_simplify_point_set \
                                                        (points->begin(),points->end(), \
                                              desc.randomSimplifyPercentage), points->end());}}

                switch(desc.order)
                {
                case InputProcessingDescriptor::EGridOutliersRandom:
                    CLEAN_GRID;
                    CLEAN_OUTLIERS;
                    CLEAN_RANDOM;
                    break;
                case InputProcessingDescriptor::EGridRandomOutliers:
                    CLEAN_GRID;
                    CLEAN_RANDOM;
                    CLEAN_OUTLIERS;
                    break;
                case InputProcessingDescriptor::EOutliersGridRandom:
                    CLEAN_OUTLIERS;
                    CLEAN_GRID;
                    CLEAN_RANDOM;
                    break;
                case InputProcessingDescriptor::EOutliersRandomGrid:
                    CLEAN_OUTLIERS;
                    CLEAN_RANDOM;
                    CLEAN_GRID;
                    break;
                case InputProcessingDescriptor::ERandomGridOutliers:
                    CLEAN_RANDOM;
                    CLEAN_GRID;
                    CLEAN_OUTLIERS;
                    break;
                case InputProcessingDescriptor::ERandomOutliersGrid:
                    CLEAN_RANDOM;
                    CLEAN_OUTLIERS;
                    CLEAN_GRID;
                    break;
                default:
                    assert(0);
                }

                Inform("" + ToString(points->size()/float(initialSize)*100) + "% of points remain (" +
                       ToString(points->size()) + "/" + ToString(initialSize)+")");
                #undef CLEAN_OUTLIERS
                #undef CLEAN_GRID
                #undef CLEAN_RANDOM
            }
        }

        std::vector<alg::Point> ProcessPoints(const cv::Mat& mat, InputProcessingDescriptor desc)
        {
            assert(mat.type() == CV_32FC3);

            if(!desc.validate())
                throw std::runtime_error("Could not validate input processor.");
            if(mat.rows*mat.cols==0 || mat.data==nullptr)
                throw std::runtime_error("Invalid mat");

            auto points = detail::RemoveDuplicatesWithGrid(mat, desc.gridSize);

            detail::CleanPointData(&points, desc);
            return points;
        }
    }
}
