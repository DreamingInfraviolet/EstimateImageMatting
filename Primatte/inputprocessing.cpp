#include "inputprocessing.h"
#include <vector>
#include <set>
#include "pixel.h"
#include "cgal.h"
#include <CGAL/remove_outliers.h>
#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/random_simplify_point_set.h>

namespace anima
{
    namespace ia
    {
        namespace detail
        {
            /** Finds list of unique pixels. */
        std::set<PixelRGB8> RemoveDuplicatePixels(const std::vector<PixelRGB8>& pixels, int bitsToIgnore)
        {
            Inform("Removing duplicate pixels...");

            PixelRGB8::comparisonBitsToIgnore(bitsToIgnore);
            std::set<PixelRGB8> pixset (pixels.begin(), pixels.end());

            Inform("" + ToString(pixset.size()/float(pixels.size())*100) +
                   "% of pixels remain (" + ToString(pixset.size())+"/" + ToString(pixels.size())+")");
            return pixset;
        }

            /** Converts pixels to points. */
            std::vector<alg::Point> PixelsToPoints(const std::set<PixelRGB8> &pixels)
            {
                std::vector<alg::Point> points;
                points.reserve(pixels.size());
                for(auto px = pixels.begin(); px!=pixels.end(); ++px)
                        points.push_back(alg::Point(px->rf(), px->gf(), px->bf()));
                return points;
            }

            void CleanPointData(std::vector<alg::Point>* points, InputProcessingDescriptor desc)
            {
                Inform("Cleaning point data...");
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

        std::vector<alg::Point> ProcessPixels(const std::vector<PixelRGB8>& pixels, InputProcessingDescriptor desc)
        {
            if(!desc.validate())
                throw std::runtime_error("Could not validate input processor.");
            if(pixels.size()==0)
                return std::vector<alg::Point>();

            START_TIMER(t);

            auto pixelss = detail::RemoveDuplicatePixels(pixels, desc.comparisonBitsToIgnore);
            END_TIMER(t);

            std::vector<alg::Point> points = detail::PixelsToPoints(pixelss);

            //std::vector<alg::Point> points = detail::PixelsToPoints(detail::RemoveDuplicatePixels(pixels, desc.comparisonBitsToIgnore));
            detail::CleanPointData(&points, desc);
            return points;
        }
    }
}
