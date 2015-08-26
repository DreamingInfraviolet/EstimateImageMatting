#include "averagebackgroundcolourlocators.h"
#include "io.h"

namespace anima
{
    namespace ia
    {
        math::vec3 ABCL_BarycentreBased::findColour(const cv::Mat& mat) const
        {
            int r = mat.rows, c = mat.cols, s = mat.step;

            math::vec3 background;

            for (unsigned i = 0; i < r; ++i)
            {
                float* data = (float*)(mat.data + s*i);

                for(unsigned j = 0; j < c; ++j)
                {
                    math::vec3& v = *((math::vec3*)(data + j*3));
                    background+=v;
                }
            }

            background/=r*c;
            return background;
        }
    }
}
