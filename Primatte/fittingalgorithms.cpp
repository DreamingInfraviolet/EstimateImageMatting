#include "fittingalgorithms.h"
#include "io.h"
#include "geomhelper.h"

namespace anima
{
    namespace alg
    {
        namespace primatte
        {
            void Test2Fitting::fit(Polyhedron& poly, const Point centre,
                                   const std::vector<Point>& points) const
            {
                math::vec3 vcentre(centre.x(), centre.y(), centre.z());

                for(Polyhedron::Vertex_iterator it = poly.vertices_begin(); it!=poly.vertices_end(); ++it)
                {
                    Point& point = it->point();
                    math::vec3 vpoint(point.x(),point.y(),point.z());
                    /////////////////////////////////////////////////////////////////////
#define STRING_POINT(p) ("("+ToString(p.x())+ " "+ToString(p.y())+ " "+ToString(p.z())+ ")")
#define STRING_VEC(p) ("("+ToString(p.x)+ " "+ToString(p.y)+ " "+ToString(p.z)+ ")")

//                    //Find all adjacent points
//                    std::vector<Point> adjPoints;
//                    auto startHalfEdge = it->vertex_begin();
//                    adjPoints.push_back(Point((*(*(*startHalfEdge).next()).vertex()).point()));
//                    for(auto itHalfEdge = ++it->vertex_begin(); itHalfEdge!=startHalfEdge; ++itHalfEdge)
//                        adjPoints.push_back(Point((*(*(*startHalfEdge).next()).vertex()).point()));
//                    //Done


                    float moveAmount = 0.01f;
                    math::vec3 moveNormal = (vcentre - vpoint).normalize();
                    const math::vec3 vec = moveNormal*moveAmount;
                    //If already at centre, forget it
                    if(vcentre.distanceSquared(vpoint+vec) > vcentre.distanceSquared(vpoint))
                        break;

                    //Try to move vertex until all points are no longer inside
                    int counter = 0;

                    while(true)
                    {
                        Tree* tree = FindTreeFromPolyhedron(poly);
                        //Check if every point is inside the sphere
                        bool allPointsInside = true;
                        for(auto itPoint = points.begin(); itPoint != points.end(); ++itPoint)
                            if(InterData::findIntersection(centre, *itPoint, tree).intersects)
                            {
                                allPointsInside = false;
                                break;
                            }
                        delete tree;

                        if(!allPointsInside || counter++==50) break;

                        point = Point
                            (
                                point.x() + vec.x,
                                point.y() + vec.y,
                                point.z() + vec.z
                            );

                    }

                    point = Point
                        (
                            point.x() - vec.x,
                            point.y() - vec.y,
                            point.z() - vec.z
                        );



                    /////////////////////////////////////////////////////////////////////
                }
            }

            /////////////////////////////////////////////////////////////////////////////////

            void TestVertexFitting::fit(Polyhedron& poly, const Point centre,
                                        const std::vector<Point>& points) const
            {
                math::vec3 centreV(centre.x(), centre.y(), centre.z());

                for(auto it = poly.vertices_begin(); it!=poly.vertices_end(); ++it)
                {
                    Point& p = it->point();
                    math::vec3 vertex(p.x(),p.y(),p.z());

                    float closestDistanceSquared =
                            vertex.distanceSquared(math::vec3(points.begin()->x(),points.begin()->y(),
                                                              points.begin()->z()));
                    //Find closest point the dumb way
                    {
                        for(auto it = points.begin()+1; it!=points.end(); ++it)
                        {
                            math::vec3 point(it->x(), it->y(), it->z());
                            float newDistance = point.distanceSquared(vertex);
                            if(newDistance < closestDistanceSquared)
                                closestDistanceSquared = newDistance;
                        }
                    }

                    float closestPointDistance = sqrt(closestDistanceSquared);

                    math::vec3 vec = centreV - math::vec3(p.x(), p.y(), p.z());
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

            void NoFitting::fit(Polyhedron& poly, const Point centre,
                                        const std::vector<Point>& points) const
            {
                return;
            }
        }
    }
}
