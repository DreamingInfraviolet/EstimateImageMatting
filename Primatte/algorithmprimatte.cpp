#include "algorithmprimatte.h"
#include "ialphalocator.h"
#include "icoloursegmenter.h"
#include "ifittingalgorithm.h"
#include "inputassembler.h"

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            AlgorithmPrimatte::AlgorithmPrimatte(AlgorithmPrimatteDesc desc)
            {
                BoundingPolyhedron poly (desc.boundingPolyhedronDesc);

                mPolys[POLY_INNER] = poly;
                mPolys[POLY_MIDDLE] = poly;
                mPolys[POLY_OUTER] = poly;

                if(!desc.segmenter)
                    throw std::runtime_error("Null segmenter");

                if(!desc.input)
                    throw std::runtime_error("Null input assember");

                if(!desc.alphaLocator)
                    throw std::runtime_error("Null alpha locator");

                mDesc = desc;
            }

            /** Prepares alpha computation for the given data set. */
            void AlgorithmPrimatte::prepare()
            {
                mPolys[POLY_OUTER].positionAround(mDesc.input->points());
                mPolys[POLY_OUTER].fit(mDesc.input->points(), mPolys[POLY_OUTER].centre());

                auto middlePoints = mDesc.segmenter->segment(mDesc.input->points(), mDesc.backgroundPoint, 0.9f);
                mPolys[POLY_MIDDLE].positionAround(middlePoints);
                mPolys[POLY_MIDDLE].fit(middlePoints, mDesc.backgroundPoint);

                auto innerPoints = mDesc.segmenter->segment(mDesc.input->points(), mDesc.backgroundPoint, 0.8f);
                mPolys[POLY_INNER].positionAround(innerPoints);
                mPolys[POLY_INNER].fit(innerPoints, mDesc.backgroundPoint);
            }

            /** Computes the alpha for the given set of points. */
            std::vector<float> AlgorithmPrimatte::computeAlphas(const std::vector<Point>& input)
            {
                return mDesc.alphaLocator->findAlphas(mPolys, POLY_COUNT, mDesc.input->points(), mDesc.backgroundPoint);
            }

            void AlgorithmPrimatte::debugDraw()
            {
                mPolys[POLY_OUTER].debugDraw(math::vec3i(0,0,0));
                mPolys[POLY_MIDDLE].debugDraw(math::vec3i(0,255,0));
                mPolys[POLY_INNER].debugDraw(math::vec3i(255,0,0));
            }
        }
    }
}