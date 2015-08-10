#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H
#include "iboundingobject.h"

class BoundingSphere : public IBoundingObject
{
private:
    std::vector<Point>* mPoints;
    Polyhedron mSphere;

public:
    BoundingSphere();

    /** Does basic initialisation. */
    virtual bool initialise(std::vector<Point>* points,
                            const std::string& polyhedronPath,
                            const float tolerance) ;

    /** Fits the shape around the points if needed. */
    virtual bool fit();

    /** Returns a mesh representation. */
    virtual Polyhedron& polyhedron();
};

#endif // BOUNDINGSPHERE_H
