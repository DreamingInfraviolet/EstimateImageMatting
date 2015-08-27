#include "algorithmprimatte.h"
#include "ialphalocator.h"
#include "icoloursegmenter.h"
#include "ifittingalgorithm.h"
#include "inputassembler.h"
#include <stdexcept>
#include "io.h"


namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            AlgorithmPrimatte::AlgorithmPrimatte(AlgorithmPrimatteDesc desc)
            {
                //Validate desc
                if(!desc.segmenter)
                    throw std::runtime_error("Null segmenter");

                if(!desc.alphaLocator)
                    throw std::runtime_error("Null alpha locator");

                mDesc = desc;
            }

            void AlgorithmPrimatte::analyse()
            {
                if(!mInput)
                    throw std::runtime_error("Using algorithm with null input.");


                //Set up unit polyhedron
                BoundingPolyhedron unitPoly (mDesc.boundingPolyhedronDesc);
                mPolys[POLY_INNER] = unitPoly;
                mPolys[POLY_OUTER] = unitPoly;

                //Fit inner polyhedron around the background points
                mPolys[POLY_INNER].fitter()->shrink(
                            mPolys[POLY_INNER],
                            mDesc.segmenter->segment(
                                mInput->backgroundPoints(),
                                mInput->background(),
                                mDesc.innerShrinkingShreshold).inner,
                            mInput->background(),
                            mDesc.innerShrinkingMinDistance);

                //Scale the inner polyhedron
                mPolys[POLY_INNER] = mPolys[POLY_INNER]*mDesc.innerPostShrinkingMultiplier;

                //Find the required radius of the outer polyhedron
                //The positioning here is done only for the radius
                mPolys[POLY_OUTER].positionAround(mInput->background(), mInput->points());
                float startRadius = mPolys[POLY_OUTER].radius()*mDesc.outerExpansionStartRadiusMultiplier;
                //Ensure that it is greater than the inner polyhedron
                float innerPolyhedronMaxSize = mPolys[POLY_INNER].findLargestRadius();
                startRadius = std::max(innerPolyhedronMaxSize, startRadius);

                //Expand the sphere from the starting radius towards startingRadius+expandDelta
                mPolys[POLY_OUTER].fitter()->expand(mPolys[POLY_OUTER],
                                                    mInput->points(),
                                                    mDesc.segmenter,
                                                    mInput->background(),
                                                    startRadius,
                                                    startRadius+mDesc.outerExpandDelta);


                //Scale outer if wanted
                float maxInnerRadius = mPolys[POLY_INNER].findLargestRadius();
                float scale = mPolys[POLY_OUTER].radius() +
                        (maxInnerRadius-mPolys[POLY_OUTER].radius())*
                        (1.f-mDesc.outerScaleParameter);
                mPolys[POLY_OUTER] = mPolys[POLY_OUTER]*(scale/mPolys[POLY_OUTER].radius());
            }

            cv::Mat AlgorithmPrimatte::computeAlphas() const
            {
                return mDesc.alphaLocator->findAlphas(mPolys, POLY_COUNT, *mInput);
            }

            void AlgorithmPrimatte::debugDraw() const
            {
                for(int i = 0;  i < POLY_COUNT; ++i)
                    mPolys[i].debugDraw(math::vec3((float)((i+2)%4==0), (float)((i+2)%3==0), 0.f));
            }
        }
    }
}
