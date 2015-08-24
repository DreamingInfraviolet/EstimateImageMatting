#include "fittingalgorithms.h"
#include "io.h"

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            void Test2Fitting::fit(SpherePolyhedron& poly, const math::vec3 centre,
                                   const std::vector<math::vec3>& points) const
            {
                float minDistance = 0.1;
                float minDistanceSquared = minDistance*minDistance;

                for(auto vertex = poly.mVertices.begin(); vertex!=poly.mVertices.end(); ++vertex)
                {
                    float moveAmount = 0.01f;
                    math::vec3 moveNormal = (centre - *vertex).normalize();
                    const math::vec3 vec = moveNormal*moveAmount;
                    //If already at centre, forget it
                    if(centre.distanceSquared(*vertex+vec) < minDistanceSquared)
                        break;

                    //Try to move vertex until all points are no longer inside
                    int counter = 0;

                    while(true)
                    {
                        //Check if every point is inside the sphere
                        bool allPointsInside = true;
                        for(auto itPoint = points.begin(); itPoint != points.end(); ++itPoint)
                        {
                            //If intersects
                            math::vec3 vector = *itPoint - centre;
                            float vectorLen = vector.length();
                            math::vec3 vectorNorm = vector/vectorLen;

                            if(poly.findIntersection(vectorNorm) < vectorLen)
                            {
                                allPointsInside = false;
                                break;
                            }
                        }

                        if(!allPointsInside || counter++==50) break;

                        *vertex += vec;
                    }

                    *vertex -= vec;
                }
            }

            /////////////////////////////////////////////////////////////////////////////////


            void NoFitting::fit(SpherePolyhedron& poly, const math::vec3 centre,
                                        const std::vector<math::vec3>& points) const
            {
                return;
            }
        }
    }
}
