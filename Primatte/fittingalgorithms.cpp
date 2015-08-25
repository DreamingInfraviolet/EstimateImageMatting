#include "fittingalgorithms.h"
#include "io.h"

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            void Test2Fitting::shrink(BoundingPolyhedron& poly,
                                   const std::vector<math::vec3>& points) const
            {
                START_TIMER(Shrinking);

                poly.positionAround(points);

                float minDistance = 0.01;
                int numberOfIterations = 3;
                float startingStep = 0.05;

                float minDistanceSquared = minDistance*minDistance;

                for(int iIteration = 0; iIteration < numberOfIterations; ++iIteration)
                {
                    float startingStepSquared = startingStep*startingStep;

                    for(auto vertex = poly.mVertices.rbegin(); vertex!=poly.mVertices.rend(); ++vertex)
                    {
                        //If too close, or move distance too great
                        const float distanceCentreToVertex = poly.centre().distanceSquared(*vertex);
                        if(distanceCentreToVertex < minDistanceSquared || distanceCentreToVertex < startingStepSquared)
                            continue;

                        math::vec3 moveNormal = (poly.centre() - *vertex).normalize();
                        const math::vec3 vec = moveNormal*startingStep;

                        //Move
                        *vertex += vec;

                        //Check if every point is inside the sphere
                        bool allPointsInside = true;
                        for(auto itPoint = points.begin(); itPoint != points.end(); ++itPoint)
                        {
                            //If intersects
                            math::vec3 vector = *itPoint - poly.centre();
                            float vectorLen = vector.length();
                            math::vec3 vectorNorm = vector/vectorLen;

                            if(poly.findDistanceToPolyhedron(vectorNorm) < vectorLen)
                            {
                                allPointsInside = false;
                                break;
                            }
                        }

                        //If movement violates rule, move back.

                        if(!allPointsInside)
                            *vertex -= vec;
                    }

                    startingStep *= 0.5f;
                }

                END_TIMER(Shrinking);
            }

            void Test2Fitting::expand(BoundingPolyhedron& poly,
                                   const std::vector<math::vec3>& points) const
            {

            }

            /////////////////////////////////////////////////////////////////////////////////


            void NoFitting::shrink(BoundingPolyhedron&,
                                        const std::vector<math::vec3>&) const
            {
                return;
            }

            void NoFitting::expand(BoundingPolyhedron&,
                                        const std::vector<math::vec3>&) const
            {
                return;
            }
        }
    }
}
