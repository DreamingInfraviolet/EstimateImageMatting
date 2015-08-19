#include "alphalocator.h"
#include "io.h"
#include "cgal.h"
#include "matrixd.h"

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>

#include <fstream>

using namespace anima::alg;

typedef Kernel::FT FT;
typedef Kernel::Ray_3 Ray;
typedef Kernel::Line_3 Line;
typedef Kernel::Triangle_3 Triangle;
typedef std::vector<Triangle>::iterator Iterator;
typedef CGAL::AABB_triangle_primitive<Kernel, Iterator> Primitive;
typedef CGAL::AABB_traits<Kernel, Primitive> AABB_triangle_traits;
typedef CGAL::AABB_tree<AABB_triangle_traits> Tree;
typedef boost::optional< Tree::Object_and_primitive_id> TreeIntersection;


namespace anima
{
    namespace alg
    {
        namespace primatte
        {
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

            InterData(float dI, float dP, bool i):
                distanceToIntersection(dI),intersects(i){}

            static InterData findIntersection(const Point& start, const Point& end, float startEndLength, const Tree* tree)
            {
                InterData out;
                Ray ray(start,end);
                TreeIntersection intersectionOp = tree->any_intersection(ray);
                out.intersects = (bool)intersectionOp;

                math::vec3 vstart(start.x(), start.y(), start.z());
                math::vec3 vend(end.x(), end.y(), end.z());

                //Check if really intersects
                if(out.intersects)
                {
                    Point intersectionPoint;
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
        };

        cv::Mat AlphaRayLocator::findAlphas(
                const BoundingPolyhedron* polyhedrons,
                const size_t polyhedronCount,
                const cv::Mat& matIn,
                const Point reference) const
        {
            assert(polyhedronCount==3);
            START_TIMER(t);

                //Send ray to input point
                //Find all intersections
                //Discard (but warn) multiple intersections with the same object
                //Probably sort by distance to background
                //Use the (hopefully) 3 intersection points to interpolate.

                const unsigned r = matIn.rows, c = matIn.cols;

                cv::Mat out;
                out.create(r, c, CV_32FC1);

                //Find poly trees
                Tree* tree[3];

                for(int i = 0; i < 3; ++i)
                {
                    std::vector<Triangle> triangles;
                    alg::Point points[3];
                    for ( auto f = polyhedrons[i].polyhedron().facets_begin(); f != polyhedrons[i].polyhedron().facets_end(); ++f)
                    {
                            alg::Polyhedron::Halfedge_around_facet_const_circulator j= f->facet_begin();
                            int i = 0;
                            do
                            {
                                points[i++] = j->vertex()->point();
                            } while(++j != f->facet_begin());
                            triangles.push_back(Triangle(points[0],points[1],points[2]));
                    }

                    tree[i] = new Tree(triangles.begin(), triangles.end());
                }

                math::vec3 vreference(reference.x(),reference.y(),reference.z());


                //For each point, send rays
                for (unsigned i = 0; i < r; ++i)
                {
                    for(unsigned j = 0; j < c; ++j)
                    {
                        cv::Point3f p = matIn.at<cv::Point3f>(i,j);
                        float alpha;


                        Point point(p.x,p.y,p.z);

                        //If right in the middle
                        if(reference==point)
                        {
                            alpha = 0;
                        }
                        else
                        {
                            //Send ray to middle
                            float startEndLength = math::vec3(p.x, p.y, p.z).distance(vreference);
                            InterData middleInter = InterData::findIntersection(reference, point, startEndLength, tree[1]);

                            if(!middleInter.intersects)
                            {
                                //If no intersection with middle, try inner:
                                InterData innerInter = InterData::findIntersection(reference, point,startEndLength, tree[0]);
                                if(!innerInter.intersects)
                                {
                                    //If no inner intersection, totally inside.
                                    alpha = 0;
                                }
                                else
                                {
                                    //If inner intersection, somewhere in the middle.
                                    alpha = (startEndLength - innerInter.distanceToIntersection) /
                                            (middleInter.distanceToIntersection - innerInter.distanceToIntersection);
                                }
                            }
                            else
                            {
                                //If intersects with middle, it's outside. Alpha = 1.
                                alpha = 1;
                            }
                        }

                        out.at<float>(i,j) = alpha;
                    }
                }

                for(int i = 0; i < 3; ++i)
                    delete tree[i];

                END_TIMER(t);

                return out;
            }

        //////////////////////////////////////////////////////////////
        cv::Mat AlphaDistanceLocator::findAlphas(
                const BoundingPolyhedron* polyhedrons,
                const size_t polyhedronCount,
                const cv::Mat& matIn,
                const Point reference) const
            {
                const unsigned r = matIn.rows, c = matIn.cols;

                cv::Mat out;
                out.create(r, c, CV_32FC1);

                for (unsigned i = 0; i < r; ++i)
                {
                    for(unsigned j = 0; j < c; ++j)
                    {
                        cv::Point3f p = matIn.at<cv::Point3f>(i,j);
                        float alpha;

                        //Distance based alpha:
                        const float distanceFullAlpha = 0.4f, distanceNoAlpha = 0.2f;
                        float a = p.x - reference.x();
                        float b = p.y - reference.y();
                        float c = p.z - reference.z();
                        float distance = sqrt(a*a+b*b+c*c);

                        alpha = (distance - distanceNoAlpha) / (distanceFullAlpha - distanceNoAlpha);
                        alpha = alpha<0 ? 0:alpha;
                        alpha = alpha>1 ? 1:alpha;

                        out.at<float>(i,j) = alpha;
                    }
                }
                return out;
            }
        }
    }
}
