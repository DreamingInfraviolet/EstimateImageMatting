#ifndef PREVIEWER_H
#define PREVIEWER_H
#include <QGLViewer/qglviewer.h>
#include "imageref.h"
#include "cgal.h"
#include "pixel.h"
#include <opencv2/opencv.hpp>
#include "ialphagenerator.h"
#include "colourhistogram.h"

class Application: public QGLViewer
{
public:
    Application();
    virtual void draw();
    void drawBackground();
    virtual void init();

    //Loads the image, calculates the histogram and converts it to points.
    bool initialiseImage();

    //Finds the alpha image.
    cv::Mat findAlpha();

    virtual QString helpString() const;

    cv::Mat mImage;
    ImageRef<PixelRGB8> mImageRef;
    CGALRgbColourHistogram<PixelRGB8> mHistogram;
    std::vector<Point> mPoints;
    IAlphaGenerator* mGenerator;

};

#endif // PREVIEWER_H
