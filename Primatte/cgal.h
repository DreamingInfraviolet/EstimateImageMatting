#pragma once
#include <CGAL/Simple_cartesian.h>
#include <CGAL/IO/Geomview_stream.h>
#include <CGAL/IO/Polyhedron_geomview_ostream.h>
#include "CGAL/Circle_3.h"
#include "CGAL/Min_sphere_of_spheres_d.h"
#include "CGAL/Min_sphere_of_spheres_d_traits_3.h"
#include "CGAL/Cartesian_d.h"
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>
#include <vector>


#include "CGAL/Polyhedron_items_3.h"
#include "CGAL/HalfedgeDS_list.h"
#include "CGAL/Polyhedron_3.h"



/** A set of useful definitions and functions to help using the CGAL library. */

namespace anima
{
    namespace alg
    {
        typedef float Real;
        typedef CGAL::Simple_cartesian<Real> Kernel;
        typedef CGAL::Sphere_3<Kernel> Sphere;

        typedef CGAL::Min_sphere_of_spheres_d_traits_3<Kernel, Real> MinSphereTraits;
        typedef CGAL::Min_sphere_of_spheres_d<MinSphereTraits> MinSphere;
        typedef MinSphereTraits::Sphere MSTSphere;
        typedef CGAL::Point_3<Kernel> Point;


        typedef CGAL::Polyhedron_traits_3<Kernel> PolyhedronTraits;
        typedef CGAL::Polyhedron_3<PolyhedronTraits> Polyhedron;

        typedef Kernel::FT FT;
        typedef Kernel::Ray_3 Ray;
        typedef Kernel::Line_3 Line;
        typedef Kernel::Triangle_3 Triangle;
        typedef std::vector<Triangle>::iterator Iterator;
        typedef CGAL::AABB_triangle_primitive<Kernel, Iterator> Primitive;
        typedef CGAL::AABB_traits<Kernel, Primitive> AABB_triangle_traits;
        typedef CGAL::AABB_tree<AABB_triangle_traits> Tree;
        typedef boost::optional< Tree::Object_and_primitive_id> TreeIntersection;



        Sphere* FindBoundingSphere(const std::vector<Point>& points, Real collisionRadius);
    }
}
