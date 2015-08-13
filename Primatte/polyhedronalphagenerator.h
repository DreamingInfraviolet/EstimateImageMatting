#pragma once
#include "ialphagenerator.h"
#include "boundingpolyhedron.h"
#include "cgal.h"

struct PolyhedronAlphaGeneratorData : IAlphaGeneratorData
{
    //Initialised unfitted bounding object.
    BoundingPolyhedronData boundingPolyhedronData;

    PolyhedronAlphaGeneratorData() :
        IAlphaGeneratorData(EAlphaGeneratorPolyhedron) {}
};

class PolyhedronAlphaGenerator : public IAlphaGenerator
{
    friend class IAlphaGenerator;
    std::vector<Point>* mPoints;
    IPolyhedronFittingAlgorithm* mFitter;

    PolyhedronAlphaGenerator();

    BoundingPolyhedron mInnerPoly, mMiddlePoly, mOuterPoly;

public:

    virtual bool initialise(IAlphaGeneratorData* data);

    /** Analyses the data, preparing alpha lookup. */
    virtual bool analyse();

    /** Takes a list of points and returns the corresponding alpha values. */
    virtual std::vector<float> findAlphas(const std::vector<Point>& points);

    virtual void debugDraw();
};
