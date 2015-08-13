#pragma once
#include <vector>
#include <cgal.h>
#include "ifittingalgorithm.h"
#include "icoloursegmenter.h"
/**
  * This class defines an interface to take in a set of image
  * colour data, analyse it, and predict the alpha value of any
  * pixel supplied.
  */

enum AlphaGeneratorType { EAlphaGeneratorPolyhedron, EAlphaGeneratorUndefined };

struct IAlphaGeneratorData
{
    const AlphaGeneratorType type;
    std::vector<Point>* points;

    IColourSegmenter* segmenter;

    IAlphaGeneratorData() : type(EAlphaGeneratorUndefined) {}
    IAlphaGeneratorData(AlphaGeneratorType type) : type(type) {}

    virtual ~IAlphaGeneratorData(){}
};

class IAlphaGenerator
{
protected:

public:

    //Creates the corresponding alpha generator based on the data passed in.
    static IAlphaGenerator* create(IAlphaGeneratorData* data);

    virtual bool initialise(IAlphaGeneratorData* data) = 0;

    /** Analyses the data, preparing alpha lookup. */
    virtual bool analyse() = 0;

    /** Takes a list of points and returns the corresponding alpha values. */
    virtual std::vector<float> findAlphas(const std::vector<Point>& points) = 0;

    virtual void debugDraw() = 0;
};
