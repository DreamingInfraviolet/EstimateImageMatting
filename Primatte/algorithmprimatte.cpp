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

                //Fill in the polyhedron objects:
                BoundingPolyhedron poly (mDesc.boundingPolyhedronDesc, mInput->background());
                for(int i = 0; i < POLY_COUNT; ++i)
                    mPolys[i] = poly;

                auto subset1 = mDesc.segmenter->segment(mInput->points(), mInput->background(), 0.3f);
                auto subset2 = mDesc.segmenter->segment(subset1, mInput->background(), 0.25f);
                auto subset3 = mDesc.segmenter->segment(subset2, mInput->background(), 0.08f);

                mPolys[POLY_INNER].shrink(subset3);

                subset2.insert(subset2.end(), mPolys[POLY_INNER].mVertices.begin(),mPolys[POLY_INNER].mVertices.end());
                mPolys[POLY_MIDDLE].shrink(subset2);

                subset1.insert(subset1.end(), mPolys[POLY_MIDDLE].mVertices.begin(),mPolys[POLY_MIDDLE].mVertices.end());
                mPolys[POLY_OUTER].shrink(subset1);
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
