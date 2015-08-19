#include "boundingpolyhedron.h"
#include "polyhedronloader.h"
#include "io.h"
#include <memory>
#include "stdlib.h"
#include <algorithm>
#include "drawhelpers.h"

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

                if(desc.meshPath == nullptr)
                    throw std::runtime_error("Null mesh path");

                mDesc = desc;

                SMeshLib::IO::importOBJ(desc.meshPath, &mPoly);

                mInitialised = true;
            }

            void BoundingPolyhedron::positionAround(const std::vector<Point>& points)
            {
                if(points.size()==0)
                    return;

                if(!mInitialised)
                    throw::std::runtime_error("Using uninitialised bounding polyhedron");
                //Find bounding sphere
                std::unique_ptr<Sphere> bounding (FindBoundingSphere(points, 0.01));
                if(!bounding.get())
                    throw std::runtime_error("Could not find bounding sphere.");

                mCentre = bounding->center();
                mInitialRadius = sqrt(bounding->squared_radius());

                Real multiplier = mInitialRadius*2*mDesc.scaleMultiplier;

                //Scale and transform mesh vertices
                for(auto it = mPoly.vertices_begin(); it!=mPoly.vertices_end(); ++it)
                {
                    Point& p = it->point();
                    p = Point
                        (
                            p.x()*multiplier+bounding->center().x(),
                            p.y()*multiplier+bounding->center().y(),
                            p.z()*multiplier+bounding->center().z()
                        );
                }

//                //Vertify that each face is a triangle
//                for(auto it = mPoly.facets_begin(); it!=mPoly.facets_end(); ++it)
//                    if(!it->is_triangle())
//                        throw std::runtime_error("Polyhedron has non-triangular faces.");
            }

            Real BoundingPolyhedron::initialRadius() const
            {
                if(!mInitialised)
                    throw::std::runtime_error("Using uninitialised bounding polyhedron");
                return mInitialRadius;
            }

            void BoundingPolyhedron::debugDraw(math::vec3i colour) const
            {
                if(!mInitialised)
                    throw::std::runtime_error("Using uninitialised bounding polyhedron");
                DrawPolyhedron(mPoly, colour.x, colour.y, colour.z);
            }

            void BoundingPolyhedron::fit(const std::vector<Point> &points, Point centre)
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
