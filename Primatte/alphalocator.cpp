#include "alphalocator.h"
#include "io.h"
#include "matrixd.h"
#include <fstream>

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
                START_TIMER(t);

                const cv::Mat& mat = input.mat();
                const unsigned r = input.mat().rows, c = input.mat().cols;
                const math::vec3 background = input.background();

                cv::Mat out;
                out.create(r, c, CV_32FC1);

                const SpherePolyhedron& middlePoly = polyhedrons[1].polyhedron();
                const SpherePolyhedron& innerPoly = polyhedrons[0].polyhedron();

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
                            //Send ray to middle
                            const float distanceCentreToPoint = point.distance(background);
                            const math::vec3 vector = point - background;
                            const float vectorLen = vector.length();
                            const math::vec3 vectorNorm = vector/vectorLen;
                            const float distanceCentreToMiddlePoly = middlePoly.findIntersection(vectorNorm);

                            //If does not intersect with middle
                            if(distanceCentreToPoint < distanceCentreToMiddlePoly)
                            {
                                //If no intersection with middle, try inner:
                                float distanceCentreToInnerPoly = innerPoly.findIntersection(vectorNorm);

                                //If does not intersect with middle, totally inside.
                                if(distanceCentreToPoint < distanceCentreToInnerPoly)
                                    alpha = 0;
                                else //If inner intersection, somewhere in the middle.
                                    alpha = (vectorLen - distanceCentreToInnerPoly) /
                                            (distanceCentreToMiddlePoly - distanceCentreToInnerPoly);
                            }
                            else //If intersects with middle, it's outside. Alpha = 1.
                                alpha = 1;
                        }

                        *(dataOut+j) = alpha;
                    }
                }

                END_TIMER(t);

                return out;
            }
        }
    }
}
