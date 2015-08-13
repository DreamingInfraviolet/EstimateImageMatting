#pragma once
#include "cgal.h"
#include <vector>
/**
  * Given a vector of points, this class must return a subset of those points.
  * based on their distance from a reference point and the given data parameters.
  */

class IColourSegmenter
{
public:
    virtual std::vector<Point> segment(const std::vector<Point>& points) = 0;
};
