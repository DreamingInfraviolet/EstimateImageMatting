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
                mPolys[POLY_INNER] = poly;

                mPolys[POLY_INNER].shrink(mDesc.segmenter->segment(mInput->backgroundPoints(),
                                                                   mInput->background(), 0.15));

                mPolys[POLY_OUTER] = poly;
//                mPolys[POLY_OUTER].expand(mInput->points());
//                mPolys[POLY_OUTER] = mPolys[POLY_OUTER]*0.5f;
                mPolys[POLY_OUTER].shrink(mInput->points());
                mPolys[POLY_OUTER] = mPolys[POLY_OUTER]*0.6f;



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
