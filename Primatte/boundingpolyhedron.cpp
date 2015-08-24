#include "boundingpolyhedron.h"
#include "io.h"
#include <memory>
#include "stdlib.h"
#include <algorithm>
#include <stdexcept>

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            BoundingPolyhedron::BoundingPolyhedron(BoundingPolyhedronDescriptor desc)
            {
                if(desc.fitter==nullptr)
                    throw std::runtime_error("Null fitter");

                mDesc = desc;

                mPoly = SpherePolyhedron(desc.phiFaces, desc.thetaFaces);

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
                    //Find bounding sphere
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

                    pointRadius = std::max(dx, std::max(dy,dz)) / 2.f;
                    pointCentre = math::vec3(maxx+minx,maxy+miny,maxz+minz) / 2.f;
                }

                mInitialRadius = (pointRadius + mDesc.centre.distance(pointCentre))*mDesc.scaleMultiplier;;
                mPoly.setCentreAndRadius(mDesc.centre, mInitialRadius);
            }

            float BoundingPolyhedron::initialRadius() const
            {
                if(!mInitialised)
                    throw::std::runtime_error("Using uninitialised bounding polyhedron");
                return mInitialRadius;
            }

            void BoundingPolyhedron::debugDraw(math::vec3 colour) const
            {
                if(!mInitialised)
                    throw::std::runtime_error("Using uninitialised bounding polyhedron");
                mPoly.debugDraw(colour);
            }

            void BoundingPolyhedron::fit(const std::vector<math::vec3> &points, math::vec3 centre)
            {
                if(points.size()==0)
                    return;
                if(!mInitialised)
                    throw::std::runtime_error("Using uninitialised bounding polyhedron");
                mDesc.fitter->fit(mPoly, centre, points);
            }
        }
    }
}
