#ifndef BOUNDINGOBJECT_H
#define BOUNDINGOBJECT_H
#include "cgal.h"
#include "matrixd.h"

/**
  * A base struct. Extend for passing custom data.
  */


struct IBoundingObjectData
{
    virtual ~IBoundingObjectData(){}
};


/**
  * This interface defines the requirements of a bounding object in the 3D
  * colourspace (e.g., a sphere!). Override it and implement the methods.
  * Assumes normalised [0,1] pixel colours.
  */


class IBoundingObject
{
public:

    virtual ~IBoundingObject() {}

    /** Does basic initialisation. */
    virtual bool initialise(IBoundingObjectData* data) = 0;

    /** Fits the object ever more around the points.
        returns true if succeeded. */
    virtual bool fit(std::vector<Point>* points) = 0;

    /** Returns whether the object has already been fitted */
    virtual bool fitted() = 0;

    /** Draws the object in the 3D viewport */
    virtual void debugDraw() = 0;
};

#endif // BOUNDINGOBJECT_H
