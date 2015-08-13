#include "cgal.h"
#include <algorithm>
#include "io.h"

Sphere* FindBoundingSphere(const std::vector<Point>& points, Real collisionRadius)
{
    Inform("Computing bounding sphere");

    //CGAL version:
    std::vector<MSTSphere> spheres;
    spheres.reserve(points.size());

    for (auto itPoint = points.begin(); itPoint != points.end(); ++itPoint)
        spheres.emplace_back(*itPoint, collisionRadius);
    MinSphere ms(spheres.begin(), spheres.end());
    if(!ms.is_valid())
        return nullptr;
    else
    {
        std::vector<Real> coordComponents;

        for(auto it = ms.center_cartesian_begin(); it!=ms.center_cartesian_end(); ++it)
            coordComponents.push_back(*it);

        assert(coordComponents.size()==3);

        return new Sphere(Point(coordComponents[0],coordComponents[1],coordComponents[2]),
                          ms.radius()*ms.radius());
    }


//    Custom version - might test later for performance.
//    Might be wrong
//    using namespace std;

//    //Generate bounding square and derive circle from that
//    Real minX = 1, minY = 1, minZ = 1, maxX = 0, maxY = 0, maxZ = 0;

//    for(auto it = points.begin(); it!=points.end(); ++it)
//    {
//        minX = min(minX, it->x());
//        minY = min(minY, it->y());
//        minZ = min(minZ, it->z());

//        maxX = max(maxX, it->x());
//        maxY = max(maxY, it->y());
//        maxZ = max(maxZ, it->z());
//    }

//    Real dx = maxX-minX;
//    Real dy = maxY-minY;
//    Real dz = maxZ-minZ;

//    Real cx = minX + dx/2;
//    Real cy = minY + dy/2;
//    Real cz = minZ + dz/2;
//    Real r  = max(dx,max(dy,dz))/2;

//    return new Sphere(Point(cx,cy,cz), r*r);


}
