#include "coloursegmenters.h"
#include "io.h"
namespace anima
{
    namespace alg
    {
        namespace primatte
        {
             std::vector<math::vec3> DistanceColourSegmenter::segment(const std::vector<math::vec3> &points,
                                                                 const math::vec3 reference,
                                                                 float approximateRadius) const
            {
                std::vector<math::vec3> out;

                float radiusSquared = approximateRadius*approximateRadius;

                for(auto it = points.begin(); it!=points.end(); ++it)
                {
                    if(reference.distanceSquared(*it) <= radiusSquared)
                        out.push_back(*it);
                }
                return out;
            }
        }
    }
}
