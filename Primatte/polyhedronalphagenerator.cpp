#include "polyhedronalphagenerator.h"

PolyhedronAlphaGenerator::PolyhedronAlphaGenerator()
{
}

bool PolyhedronAlphaGenerator::initialise(IAlphaGeneratorData* data)
{
    try
    {
        PolyhedronAlphaGeneratorData* nativeData =
                dynamic_cast<PolyhedronAlphaGeneratorData*>(data);

        BoundingPolyhedron poly;
        if(!poly.initialise(&nativeData->boundingPolyhedronData))
            return false;

        mInnerPoly = poly;
        mOuterPoly = poly;
        mMiddlePoly = poly;
        mPoints = nativeData->points;
        if(!mPoints)
            return false;

    }
    catch(...)
    {
        return false;
    }
    return true;
}

void PolyhedronAlphaGenerator::debugDraw()
{
    mInnerPoly.debugDraw();
    mOuterPoly.debugDraw();
    mMiddlePoly.debugDraw();
}



/** Analyses the data, preparing alpha lookup. */
bool PolyhedronAlphaGenerator::analyse()
{
    mInnerPoly.fit(mPoints);
    return true;
}

/** Takes a list of points and returns the corresponding alpha values. */
std::vector<float> PolyhedronAlphaGenerator::findAlphas(const std::vector<Point>& points)
{
    return std::vector<float>();
}
