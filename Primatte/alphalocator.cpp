#include "alphalocator.h"

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            std::vector<float> AlphaDistanceLocator::findAlphas(
                    const BoundingPolyhedron *polyhedrons,
                    const size_t polyhedroncount,
                    const std::vector<Point> &points, const Point reference) const
            {
                std::vector<float> out;

                for(auto px = points.begin(); px!=points.end(); ++px)
                {
                    //Distance based alpha:
                    float distanceFullAlpha = 0.4f, distanceNoAlpha = 0.2f;
                    float a = (float)px->x()- reference.x();
                    float b = (float)px->y()- reference.y();
                    float c = (float)px->z()- reference.z();
                    float distance = sqrt(a*a+b*b+c*c)/255.f;

                    float ans = (distance - distanceNoAlpha) / (distanceFullAlpha - distanceNoAlpha);
                    ans = ans<0 ? 0:ans;
                    ans = ans>1 ? 1:ans;

                    out.push_back(ans);
                }

                return out;
            }
        }
    }
}
