#include "boundingpolyhedron.h"
#include "io.h"
#include "stdlib.h"
#include <algorithm>
#include <stdexcept>
#include "ifittingalgorithm.h"

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            BoundingPolyhedron::BoundingPolyhedron(BoundingPolyhedronDescriptor desc)
                : SpherePolyhedron(desc.phiFaces, desc.thetaFaces)
            {
                if(desc.fitter==nullptr)
                    throw std::runtime_error("Null fitter");

                if(desc.scaleMultiplier < 0)
                    throw std::runtime_error("Negative bounding polyhedron scale multiplier");

                mDesc = desc;

                mInitialised = true;
            }

            //Adapted from http://stackoverflow.com/a/24818473
            std::pair<math::vec3, float> calculateBoundSphere(const std::vector<math::vec3>& vertices)
            {
                math::vec3 center = vertices[0];
                float radius = 0.0001f;
                float len, alpha, alphaSq;

                for (int i = 0; i < 2; i++)
                {
                    for (auto it = vertices.begin(); it != vertices.end(); it++)
                    {
                        math::vec3 pos = *it;
                        math::vec3 diff = pos - center;
                        len = diff.length();
                        if (len > radius)
                        {
                            alpha = len / radius;
                            alphaSq = alpha * alpha;
                            radius = 0.5f * (alpha + 1 / alpha) * radius;
                            center =(center * (1 + 1 / alphaSq)+ pos*(1 - 1 / alphaSq)) * 0.5f;
                        }
                    }
                }

                for (auto it = vertices.begin(); it != vertices.end(); it++)
                {
                    math::vec3 diff = *it - center;
                    len = diff.length();
                    if (len > radius)
                    {
                        radius = (radius + len) / 2.0f;
                        center = center + diff * ((len - radius) / len);
                    }
                }

                return std::make_pair(center, radius);
            }


            void BoundingPolyhedron::positionAround(const math::vec3 desiredCentre, const std::vector<math::vec3>& points)
            {
                if(!mInitialised)
                    throw::std::runtime_error("Using uninitialised bounding polyhedron");

                if(points.size()==0)
                    return;

                auto bounding = calculateBoundSphere(points);

                float radius = (bounding.second + desiredCentre.distance(bounding.first))*mDesc.scaleMultiplier;
                setCentreAndRadius(desiredCentre, radius);
            }

            BoundingPolyhedron BoundingPolyhedron::operator * (const float scale)
            {
                BoundingPolyhedron out = *this;
                for(auto it = out.mVertices.begin(); it!=out.mVertices.end(); ++it)
                    *it = (*it-mCentre)*scale+mCentre;
                out.mRadius *= scale;
                return out;
            }

        }
    }
}
