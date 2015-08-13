#include "drawhelpers.h"
#include <QGLViewer/qglviewer.h>

void DrawPolyhedron(const Polyhedron& polyhedron, float cr, float cg, float cb)
{
    //Adapted from https://code.google.com/p/jeronimo/source/browse/trunk/software/sjSkeletonizer/sjViewer.cpp?r=35
    Point puntos[3], a,b,c;
    glBegin(GL_TRIANGLES);

    Point color3(cr,cg,cb);
    for ( auto f = polyhedron.facets_begin(); f != polyhedron.facets_end(); ++f)
    {
            Polyhedron::Halfedge_around_facet_const_circulator j= f->facet_begin();
            int i = 0;
            do
            {
                puntos[i++] = j->vertex()->point();
            } while(++j != f->facet_begin());

            a = puntos[0];
            b = puntos[1];
            c = puntos[2];

            //glColor3f(1.0f,1.0f,1.0f);
            glColor3f( (float)color3[0] ,(float)color3[1],(float)color3[2]);
            glVertex3f( (float)a[0] ,(float)a[1],(float)a[2]);
            glVertex3f( (float)b[0] ,(float)b[1],(float)b[2]);
            glVertex3f( (float)c[0] ,(float)c[1],(float)c[2]);
    }
    glEnd();
}
