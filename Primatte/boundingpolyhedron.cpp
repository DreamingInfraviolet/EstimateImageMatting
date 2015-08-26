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
            BoundingPolyhedron::BoundingPolyhedron(BoundingPolyhedronDescriptor desc, const math::vec3 centre)
                : SpherePolyhedron(desc.phiFaces, desc.thetaFaces)
            {
                if(desc.fitter==nullptr)
                    throw std::runtime_error("Null fitter");

                mDesc = desc;



                mCentre = centre;
                mInitialised = true;
            }

            void BoundingPolyhedron::positionAround(const std::vector<math::vec3>& points)
            {
                if(!mInitialised)
                    throw::std::runtime_error("Using uninitialised bounding polyhedron");

                float pointRadius = 1;
                math::vec3 pointCentre;

                if(points.size()!=0)
                {
                    //Find bounding box
                    float minx = 1,miny = 1,minz = 1,maxx = 0,maxy = 0,maxz = 0;

                    for(auto p = points.begin(); p!=points.end(); ++p)
                    {
                        minx = std::min(p->x, minx);
                        miny = std::min(p->y, miny);
                        minz = std::min(p->z, minz);
                        maxx = std::max(p->x, maxx);
                        maxy = std::max(p->y, maxy);
                        maxz = std::max(p->z, maxz);
                    }

                    float dx = maxx-minx;
                    float dy = maxy-miny;
                    float dz = maxz-minz;

                    //Use bounding box to compute centre and radius.
                    pointRadius = std::max(dx, std::max(dy,dz)) / 2.f;
                    pointCentre = math::vec3(maxx+minx,maxy+miny,maxz+minz) / 2.f;
                }

                mRadius = (pointRadius + mCentre.distance(pointCentre))*mDesc.scaleMultiplier;
                setCentreAndRadius(mCentre, mRadius);
            }

            void BoundingPolyhedron::shrink(const std::vector<math::vec3> &points)
            {
                if(points.size()==0)
                    return;
                if(!mInitialised)
                    throw::std::runtime_error("Using uninitialised bounding polyhedron");
                mDesc.fitter->shrink(*this, points);
            }

            void BoundingPolyhedron::expand(const std::vector<math::vec3> &points)
            {
                if(points.size()==0)
                    return;
                if(!mInitialised)
                    throw::std::runtime_error("Using uninitialised bounding polyhedron");
                mDesc.fitter->expand(*this, points);
            }


            BoundingPolyhedron BoundingPolyhedron::operator * (const float scale)
            {
                BoundingPolyhedron out = *this;
                for(auto it = out.mVertices.begin(); it!=out.mVertices.end(); ++it)
                    *it = (*it-mCentre)*scale+mCentre;
                return out;
            }
        }
    }
}
