#include "boundingsphere.h"
#include "polyhedronloader.h"
#include "io.h"
#include <memory>

BoundingSphere::BoundingSphere()
{
}

/** Does basic initialisation. */
bool BoundingSphere::initialise(std::vector<Point>* points,
                        const std::string& polyhedronPath,
                        const float tolerance)
{
    mPoints = points;

    try
    {
        //Load sphere
        Inform("Loading " + polyhedronPath);
        SMeshLib::IO::importOBJ(polyhedronPath, &mSphere);

        Inform("Applying sphere");
        //Find bounding sphere
        std::unique_ptr<Sphere> bounding =
                std::unique_ptr<Sphere>(GetBoundingSphere(*mPoints, 0.1));
        if(!bounding.get())
            return false;

        Real boundingRadius = sqrt(bounding->squared_radius());

        //Offset and scale mesh vertices
        for(auto it = mSphere.vertices_begin(); it!=mSphere.vertices_end(); ++it)
        {
            Point& p = it->point();
            p = Point
                (
                    p.x()*boundingRadius+bounding->center().x(),
                    p.y()*boundingRadius+bounding->center().y(),
                    p.z()*boundingRadius+bounding->center().z()
                );
        }
    }
    catch(const std::exception& e)
    {
        return false;
    }

    return true;
}

bool BoundingSphere::fit()
{
    return true;
}

Polyhedron& BoundingSphere::polyhedron()
{
    return mSphere;
}
