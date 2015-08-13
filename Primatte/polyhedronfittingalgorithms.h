#ifndef FITTINGALHORITHMS_H
#define FITTINGALHORITHMS_H
#include "ifittingalgorithm.h"

/**
  * Various polyhedron fitting algorithms.
  */


class TestVertexFitting : public IPolyhedronFittingAlgorithm
{
public:
    virtual void fit(Polyhedron& poly, math::vec3 centre, std::vector<Point>* points)
    {
        for(auto it = poly.vertices_begin(); it!=poly.vertices_end(); ++it)
        {
            Point& p = it->point();
            math::vec3 vertex(p.x(),p.y(),p.z());

            float closestDistanceSquared =
                    vertex.distanceSquared(math::vec3(points->begin()->x(),points->begin()->y(),
                                                      points->begin()->z()));
            //Find closest point the dumb way
            {
                for(auto it = points->begin()+1; it!=points->end(); ++it)
                {
                    math::vec3 point(it->x(), it->y(), it->z());
                    float newDistance = point.distanceSquared(vertex);
                    if(newDistance < closestDistanceSquared)
                        closestDistanceSquared = newDistance;
                }
            }

            float closestPointDistance = sqrt(closestDistanceSquared);

            math::vec3 vec = centre - math::vec3(p.x(), p.y(), p.z());
            vec.normalize();
            vec *= closestPointDistance*0.9 ;

            p = Point
                (
                    p.x() + vec.x,
                    p.y() + vec.y,
                    p.z() + vec.z
                );
        }
    }
};

#endif // FITTINGALHORITHMS_H
