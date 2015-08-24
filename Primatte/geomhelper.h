#pragma once
#include "cgal.h"
#include "matrixd.h"
struct InterData
{
    //Line width between centre and an intersection
    float distanceToIntersection;

    //Distance from centre to pint
    float distanceToPoint;

    //Whether the point lies between the centre and the first intersection
    bool intersects;

    InterData():
        distanceToIntersection(0),intersects(false){}

    InterData(float dI, bool i):
        distanceToIntersection(dI),intersects(i){}

    static InterData findIntersection(const anima::alg::Point& start, const anima::alg::Point& end, float startEndLength, const anima::alg::Tree* tree)
    {
        InterData out;
        anima::alg::Ray ray(start,end);
        anima::alg::TreeIntersection intersectionOp = tree->any_intersection(ray);
        out.intersects = (bool)intersectionOp;

        math::vec3 vstart(start.x(), start.y(), start.z());
        math::vec3 vend(end.x(), end.y(), end.z());

        //Check if really intersects
        if(out.intersects)
        {
            anima::alg::Point intersectionPoint;
            CGAL::assign(intersectionPoint,intersectionOp->first);
            math::vec3 vinter(intersectionPoint.x(),intersectionPoint.y(),intersectionPoint.z());

            out.distanceToIntersection = vstart.distance(vinter);

            if(startEndLength < out.distanceToIntersection)
            {
                //No intersection
                out.intersects = false;
            }
        }
        return out;
    }

    static InterData findIntersection(const anima::alg::Point& start, const anima::alg::Point& end, const anima::alg::Tree* tree)
    {
        return findIntersection(start, end, math::vec3(start.x(), start.y(), start.z()).distance(math::vec3(end.x(),end.y(),end.z())), tree);
    }
};

static anima::alg::Tree* FindTreeFromPolyhedron(const anima::alg::Polyhedron& poly)
{
    using namespace anima::alg;

    std::vector<Triangle> triangles;
    Point points[3];
    for ( auto f = poly.facets_begin(); f != poly.facets_end(); ++f)
    {
            Polyhedron::Halfedge_around_facet_const_circulator j= f->facet_begin();
            int i = 0;
            do
            {
                points[i++] = j->vertex()->point();
            } while(++j != f->facet_begin());
            triangles.push_back(Triangle(points[0],points[1],points[2]));
    }

    return new Tree(triangles.begin(), triangles.end());
}
