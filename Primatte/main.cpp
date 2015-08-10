#include "cgal.h"
#include <vector>
#include <thread>
#include <cassert>
#include "io.h"
#include <qt4/QtGui/qimage.h>
#include "boundingsphere.h"
#include "image.h"
#include "pixel.h"

int main()
{
    Inform("Running");

    Inform("Initialising geomview");
    CGAL::Geomview_stream gv;
    gv.set_line_width(4);
    gv.set_bg_color(CGAL::Color(0,255,255));
    gv.set_wired(true);
    gv<<CGAL::RED;

    Inform("Loading image");
    QImage qimage ("test.jpg");
    if(qimage.isNull())
        return 5;


    assert(qimage.format() == QImage::Format_RGB32);

    Image<PixelRGB8> image((unsigned char*)qimage.bits(), qimage.width(), qimage.height(),
                           3, 1, 4, 1);

    Inform(ToString(image.width()));
    Inform(ToString(image.height()));

    for(int j = 0; j < image.width();++j)
        for(int i = 0; i < image.height(); ++i)
        {
            PixelRGB8 px = image.pixelAt(i,j);
        }


    std::vector<Point> points = {Point(0,0,0), Point(0.1,0,0), Point(0.2,0.1,0.43)};
    Sphere* min = GetBoundingSphere(points, 0.00);
    if(!min)
        return 3;
    for(size_t i = 0; i < points.size(); ++i)
        gv<<points[i];
    //gv<<*min;


    BoundingSphere bSphere;
    if(!bSphere.initialise(&points, "objects/sphere.obj", 0))
        return 4;

    gv << bSphere.polyhedron();

    std::this_thread::sleep_for(std::chrono::milliseconds(100000));
}
