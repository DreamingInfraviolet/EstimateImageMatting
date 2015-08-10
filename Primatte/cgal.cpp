#include "cgal.h"


Sphere* GetBoundingSphere(const std::vector<Point>& points, Real collisionRadius)
{
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
        {
            coordComponents.push_back(*it);
        }
        assert(coordComponents.size()==3);
        return new Sphere(Point(coordComponents[0],coordComponents[1],coordComponents[2]),
                ms.radius());

    }

}
