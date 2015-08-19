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

                if(!desc.alphaLocator)
                    throw std::runtime_error("Null alpha locator");

                mDesc = desc;
            }

            /** Prepares alpha computation for the given data set. */
            void AlgorithmPrimatte::analyse()
            {
                mPolys[POLY_OUTER].positionAround(mInput->points());
                mPolys[POLY_OUTER].fit(mInput->points(), mPolys[POLY_OUTER].centre());

                auto middlePoints = mDesc.segmenter->segment(mInput->points(), mInput->background(), 0.35f);
                mPolys[POLY_MIDDLE].positionAround(middlePoints);
                mPolys[POLY_MIDDLE].fit(middlePoints, mInput->background());

                auto innerPoints = mDesc.segmenter->segment(mInput->points(), mInput->background(), 0.2f);
                mPolys[POLY_INNER].positionAround(innerPoints);
                mPolys[POLY_INNER].fit(innerPoints, mInput->background());

            }

            /** Computes the alpha for the given set of points. */
            cv::Mat AlgorithmPrimatte::computeAlphas() const
            {
                return mDesc.alphaLocator->findAlphas(mPolys, POLY_COUNT, mInput->mat(), mInput->background());
            }

            void AlgorithmPrimatte::debugDraw() const
            {
                mPolys[POLY_OUTER].debugDraw(math::vec3i(0,0,0));
                mPolys[POLY_MIDDLE].debugDraw(math::vec3i(0,255,0));
                mPolys[POLY_INNER].debugDraw(math::vec3i(255,0,0));
            }
        }
    }
}
