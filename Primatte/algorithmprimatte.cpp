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
                : mAnalysed(false)
            {
                //Validate desc
                if(!desc.segmenter)
                    throw std::runtime_error("Null segmenter");

                if(!desc.alphaLocator)
                    throw std::runtime_error("Null alpha locator");

                if(desc.innerPostShrinkingMultiplier < 0 ||
                   desc.innerShrinkingMinDistance < 0 ||
                   desc.innerShrinkingThreshold < 0 ||
                   desc.outerExpandDelta < 0 ||
                   desc.outerExpansionStartThreshold < 0 ||
                   desc.outerScaleParameter <0)
                    throw std::runtime_error("Algorithm parameter out of range");

                //Try constructing a bounding polyhedron to validate its descriptor
                BoundingPolyhedron(desc.boundingPolyhedronDesc);

                //Set desc
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
                                mDesc.innerShrinkingThreshold).inner,
                            mInput->background(),
                            mDesc.innerShrinkingMinDistance);

                //Scale the inner polyhedron
                mPolys[POLY_INNER] = mPolys[POLY_INNER]*mDesc.innerPostShrinkingMultiplier;

                //Ensure that it is greater than the inner polyhedron
                float innerPolyhedronMaxSize = mPolys[POLY_INNER].findLargestRadius();
                float startRadius = std::max(innerPolyhedronMaxSize, mDesc.outerExpansionStartThreshold);

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

                mAnalysed = true;
            }

            cv::Mat AlgorithmPrimatte::computeAlphas() const
            {
                if(!mAnalysed)
                    throw std::runtime_error("Trying to compute alphas with algorithm before input analysis.");
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
