#include "fittingalgorithms.h"
#include "io.h"
#include "icoloursegmenter.h"

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            unsigned StableFitting::countPointsInside(const std::vector<math::vec3>& points, BoundingPolyhedron& poly)
            {
                int pointsInside = 0;
                for(auto itPoint = points.begin(); itPoint != points.end(); ++itPoint)
                {
                    //If intersects
                    math::vec3 vector = *itPoint - poly.centre();
                    float vectorLen = vector.length();
                    math::vec3 vectorNorm = vector/vectorLen;

                    if(poly.findDistanceToPolyhedron(vectorNorm) >= vectorLen)
                        ++pointsInside;
                }
                return pointsInside;
            }

            void StableFitting::shrink(BoundingPolyhedron& poly,
                                      const std::vector<math::vec3>& points,
                                      math::vec3 backgroundPoint,
                                      float minimumDistance) const
            {
                START_TIMER(Shrinking);

                poly.positionAround(backgroundPoint, points);

                float step = poly.radius()/2.f;

                float minDistanceSquared = minimumDistance*minimumDistance;

                int originalPointsInside = countPointsInside(points, poly);

                for(int iIteration = 0; iIteration < mNoOfIterations; ++iIteration)
                {
                    float stepSquared = step*step;

                    for(auto vertex = poly.mVertices.rbegin(); vertex!=poly.mVertices.rend(); ++vertex)
                    {
                        //If too close, or move distance too great
                        const float distanceCentreToVertex = poly.centre().distanceSquared(*vertex);
                        if(distanceCentreToVertex < minDistanceSquared || distanceCentreToVertex < stepSquared)
                            continue;

                        math::vec3 moveNormal = (poly.centre() - *vertex).normalize();
                        const math::vec3 vec = moveNormal*step;

                        //Move
                        *vertex += vec;

                        int newPointsInside = countPointsInside(points, poly);

                        //Move back if movement violates rule
                        if(newPointsInside<originalPointsInside)
                            *vertex -= vec;
                    }

                    step *= 0.5f;
                }

                END_TIMER(Shrinking);
            }

            void StableFitting::expand(BoundingPolyhedron& poly,
                                      const std::vector<math::vec3>& points, IColourSegmenter* segmenter,
                                      math::vec3 backgroundPoint, float startRadius, float endRadius) const
            {

                START_TIMER(Expanding);

                auto innerouter = segmenter->segment(points, backgroundPoint, startRadius);

                //Indicates whether a vertex was unable to move at least once due to outer points.
                std::vector<bool> didVertexEncounterResistance;
                didVertexEncounterResistance.resize(poly.mVertices.size(), false);

                //Position the polygon around the inner points.
                poly.positionAround(backgroundPoint, innerouter.inner);

                //Make a copy that is to be scaled.
                //We need a copy to deal with the case where no resistance is met.
                BoundingPolyhedron newPoly = poly;

                //The starting step is set to be half way between the start and end distance.
                float step = (endRadius-startRadius)/2.f;

                //Count number of points outside for later reference.
                int originalPointsOutside = innerouter.outer.size()-countPointsInside(innerouter.outer, newPoly);

                //Iterate...
                for(int iIteration = 0; iIteration < mNoOfIterations; ++iIteration)
                {
                    //Try moving each vertex outwards
                    for(int iVertex = 0; iVertex < newPoly.mVertices.size(); ++iVertex)
                    {
                        //Find movement vector
                        math::vec3 moveNormal = (newPoly.mVertices[iVertex]-newPoly.centre()).normalize();
                        const math::vec3 vec = moveNormal*step;

                        //Move
                        newPoly.mVertices[iVertex] += vec;

                        //Find the number of points now outside after movement.
                        int newPointsOutside = innerouter.outer.size()-countPointsInside(innerouter.outer, newPoly);

                        //If there are now less points outside, we have gone through something. Move back and mark resistance.
                        if(newPointsOutside < originalPointsOutside)
                        {
                            newPoly.mVertices[iVertex] -= vec;
                            didVertexEncounterResistance[iVertex] = true;
                        }
                    }

                    //halve the step and try again.
                    step *= 0.5f;
                }


                //The idea here is that if a vertex did not encounter any resistance while moving,
                //Move it by the average movement of the vertices that _did_ encounter resistance.

                //Find average movement of vertices that found resistance:
                float averageMovement = 0.f;
                int divisor = 0;
                for(int i = 0; i < didVertexEncounterResistance.size(); ++i)
                    if(didVertexEncounterResistance[i])
                    {
                        averageMovement += poly.mVertices[i].distance(newPoly.mVertices[i]);
                        ++divisor;
                    }

                averageMovement/=float(divisor);

                //If a vertex found resistance, keep it. Otherwise, restore it and move by the average.
                for(int i = 0; i < didVertexEncounterResistance.size(); ++i)
                    if(didVertexEncounterResistance[i])
                        poly.mVertices[i] = newPoly.mVertices[i];
                else
                        poly.mVertices[i] = poly.mVertices[i]+(newPoly.mVertices[i]-poly.mVertices[i]).normalize()*averageMovement;

                END_TIMER(Expanding);
            }
        }
    }
}
