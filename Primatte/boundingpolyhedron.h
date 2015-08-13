#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H
#include "iboundingobject.h"
#include "matrixd.h"
#include "ifittingalgorithm.h"

struct BoundingPolyhedronData : public IBoundingObjectData
{
    std::string meshPath;
    float scaleMultiplier;
    IPolyhedronFittingAlgorithm* fitter;
};

class BoundingPolyhedron : public IBoundingObject
{
private:
    Polyhedron mPoly;
    math::vec3 mCentre;
    IPolyhedronFittingAlgorithm* mFitter;
    float mScaleMultiplier;
    bool mFitted;
public:
    BoundingPolyhedron();

    /** Does basic initialisation. */
    virtual bool initialise(IBoundingObjectData* dataIn);

    /** Fits the shapes around the points if needed. */
    virtual bool fit(std::vector<Point>* points);

    virtual void debugDraw();

    virtual bool fitted();
};

#endif // BOUNDINGSPHERE_H
