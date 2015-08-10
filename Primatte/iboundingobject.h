#ifndef BOUNDINGOBJECT_H
#define BOUNDINGOBJECT_H
#include "cgal.h"


/**
  * This interface defines the requirements of a bounding object in the 3D
  * colourspace (e.g., a sphere!). Override it and implement the methods.
  */

class IBoundingObject
{
public:

    virtual ~IBoundingObject() {}

    /** Does basic initialisation. */
    virtual bool initialise(std::vector<Point>* points,
                            const std::string& polyhedronPath,
                            const float tolerance) = 0;

    /** Fits the shape around the points if needed. */
    virtual bool fit() = 0;

    /** Returns a mesh representation. */
    virtual Polyhedron& polyhedron() = 0;
};

#endif // BOUNDINGOBJECT_H
