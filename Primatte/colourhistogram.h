#ifndef COLOURHISTOGRAM_H
#define COLOURHISTOGRAM_H
#include "imageref.h"
#include "io.h"
#include <vector>
#include "cgal.h"
#include <set>
#include "pixel.h"

/*
  * These classes are meant to act as a fast way to find the set of RGB pixels
  * that are present in a matrix. They differ in performance/memory usage.
  * The Pixel type must have r,g and b functions.
  * */
template <class Pixel>
class ColourHistogram
{
protected:
    ImageRef<Pixel>& mImage;
    bool mDirty;
    std::vector<Pixel> mHistogram;
public:
    ColourHistogram(ImageRef<Pixel>& image) : mImage(image), mDirty(true)
    {

    }

    virtual ~ColourHistogram() {}

    /** Computes a histogram of the points, essentially removing duplicates.
    */
    virtual std::vector<Pixel> findHistogram() = 0;

    /** Calculates the histogram if the dirty flag is set using findHistogram().
      * Otherwise returns internal version. Should generally be called instead of
      * findHistogram(). */
    virtual std::vector<Pixel>& histogram()
    {
        if(this->mDirty)
            this->mHistogram = this->findHistogram();
        return this->mHistogram;
    }
};


/** Uses a set to generate the histogram. Expects rgb pixels */
template<class Pixel>
class SRgbColourHistogram : public ColourHistogram<Pixel>
{
public:
    SRgbColourHistogram(ImageRef<Pixel>& image) : ColourHistogram<Pixel>(image) { }

    virtual std::vector<Pixel> findHistogram()
    {
        Inform("Finding histogram");
        std::set<Pixel> pixels;

        for(size_t y = 0; y < this->mImage.height(); ++y)
            for(size_t x = 0; x < this->mImage.width(); ++x)
                pixels.insert(this->mImage.pixelAt(x,y));

        Inform("Histogram found: " + ToString(pixels.size()) +
               " unique colours (" + ToString(pixels.size()/
                                              float(this->mImage.width()*
                                                    this->mImage.height())*100) + "%)");
        return std::vector<Pixel> (pixels.begin(), pixels.end());;
    }
};



#include <CGAL/remove_outliers.h>
#include <CGAL/grid_simplify_point_set.h>
/** Uses the CGAL library to generated a clean histogram.
  * Uses SRgbColourHistogram internally as a pre-pass. */
template<class Pixel>
class CGALRgbColourHistogram : public ColourHistogram<Pixel>
{
public:
    CGALRgbColourHistogram(ImageRef<Pixel>& image) : ColourHistogram<Pixel>(image) { }

    virtual std::vector<Pixel> findHistogram()
    {
        bool usePrepass = true;

        Inform("Finding histogram");
        //Convert colours to 3D points:
        std::vector<Point> points;

        if(usePrepass)
        {
            SRgbColourHistogram<Pixel> preHist(this->mImage);
            std::vector<Pixel> pixels = preHist.histogram();
            points.reserve(pixels.size());
            for(size_t i = 0; i < pixels.size(); ++i)
                    points.push_back(Point(pixels[i].rf(), pixels[i].gf(), pixels[i].bf()));
        }
        else
        {

            points.reserve(this->mImage.width()*this->mImage.height());
            for(size_t y = 0; y < this->mImage.height(); ++y)
                for(size_t x = 0; x < this->mImage.width(); ++x)
                {
                    Pixel px = this->mImage.pixelAt(x,y);
                    points.push_back(Point(px.rf(), px.gf(), px.bf()));
                }
        }

        //points.erase(CGAL::remove_outliers(points.begin(), points.end(), 4, 5.0), points.end());
        points.erase(CGAL::grid_simplify_point_set(points.begin(), points.end(), 0.04), points.end());



        //Convert points to pixels
        std::vector<Pixel> out;
        out.reserve(points.size());
        for(size_t i = 0; i < points.size(); ++i)
            out.push_back(Pixel((unsigned char)(points[i].x()*255),
                                (unsigned char)(points[i].y()*255),
                                (unsigned char)(points[i].z()*255)));



        Inform("Histogram found: " + ToString(out.size()) +
               " unique colours (" + ToString(out.size()/
                                              float(this->mImage.width()*
                                                    this->mImage.height())*100) + "%)");
        return out;
    }
};




#endif // COLOURHISTOGRAM_H
