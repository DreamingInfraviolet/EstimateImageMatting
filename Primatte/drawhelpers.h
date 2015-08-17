#pragma once
#include "cgal.h"

/* Some functions to help with debug drawing. */
namespace anima
{
    /** Draws a polyhedron.
      * @param polyhedron The polyhedron to be drawn.
      * @param cr The red colour value [0,1)
      * @param cg The green colour value [0,1)
      * @param cb The blue colour value [0,1) */
    void DrawPolyhedron(const alg::Polyhedron& polyhedron, float cr, float cg, float cb);

}
