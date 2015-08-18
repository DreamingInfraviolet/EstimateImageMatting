#include "coloursegmenters.h"
#include <CGAL/squared_distance_3.h>
namespace anima
{
    namespace alg
    {
        namespace primatte
        {
             std::vector<Point> DistanceColourSegmenter::segment(const std::vector<Point> &points,
                                                                 const Point reference,
                                                                 float approximateRadius) const
            {
                std::vector<Point> out;

                float radiusSquared = approximateRadius*approximateRadius;

                for(auto it = points.begin(); it!=points.end(); ++it)
                {
                    if(CGAL::squared_distance(reference, *it) <= radiusSquared)
                        out.push_back(*it);
                }
                return out;
            }
        }
    }
}
