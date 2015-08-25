#include "alphalocator.h"
#include "io.h"
#include "matrixd.h"

using namespace anima::alg;



namespace anima
{
    namespace alg
    {
        namespace primatte
        {
        cv::Mat AlphaRayLocator::findAlphas(
                const BoundingPolyhedron* polyhedrons,
                const size_t polyhedronCount,
                const ia::InputAssembler& input) const
            {
                assert(polyhedronCount==3);
                START_TIMER(AlphaLocator);

                const cv::Mat& mat = input.mat();
                const unsigned r = input.mat().rows, c = input.mat().cols;
                const math::vec3 background = input.background();

                cv::Mat out;
                out.create(r, c, CV_32FC1);

                const SpherePolyhedron& outerPoly = polyhedrons[2];
                const SpherePolyhedron& middlePoly = polyhedrons[1];
                const SpherePolyhedron& innerPoly = polyhedrons[0];

                //For each point, send rays
                for (unsigned i = 0; i < r; ++i)
                {
                    float* data = (float*)(mat.data + mat.step*i);
                    float* dataOut = (float*)(out.data + out.step*i);
                    for(unsigned j = 0; j < c; ++j)
                    {
                        math::vec3& point = *((math::vec3*)(data + j*3));
                        float alpha;

                        //If right in the middle
                        if(background==point)
                            alpha = 0;
                        else
                        {
                            //Prepare vector
                            const math::vec3 vector = point - background;
                            const float vectorLen = vector.length();
                            const math::vec3 vectorNorm = vector/vectorLen;
                            const float distanceToPoint = point.distance(background);

                            const float distanceToOuterPoly = outerPoly.findDistanceToPolyhedron(vectorNorm);

                            //If inside outer poly, alpha < 1
                            if(distanceToPoint < distanceToOuterPoly)
                            {
                                float distanceToMiddlePoly = middlePoly.findDistanceToPolyhedron(vectorNorm);

                                //If does not intersect with middle, check inner
                                if(distanceToPoint < distanceToMiddlePoly)
                                {
                                    float distanceToInnerPoly = innerPoly.findDistanceToPolyhedron(vectorNorm);

                                    //If does not intersect with middle, fully inside
                                    if(distanceToPoint < distanceToInnerPoly)
                                        alpha = 0;
                                    else //interpolate between inner and middle
                                    {
                                        alpha = (vectorLen - distanceToInnerPoly) /
                                                (distanceToMiddlePoly - distanceToInnerPoly);
                                        alpha *= 0.99f;
                                    }
                                }
                                else //Interpolate between middle and outer
                                {
                                    alpha = (vectorLen - distanceToMiddlePoly) /
                                            (distanceToOuterPoly - distanceToMiddlePoly);
                                    //Scale it into the [0.9,1] range.
                                    alpha = alpha/100.f + 0.99f;
                                }
                            }
                            else //If intersects with middle, it's outside. Alpha = 1.
                                alpha = 1;
                        }

                        *(dataOut+j) = alpha;
                    }
                }

                END_TIMER(AlphaLocator);

                return out;
            }
        }
    }
}
