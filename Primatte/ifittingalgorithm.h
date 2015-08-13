#ifndef IFITTINGALGORITHM_H
#define IFITTINGALGORITHM_H
#include "cgal.h"
#include <vector>
#include "matrixd.h"

/**
  * Defines the interface that must be implemented by a
  * bounding object fitting algorithm.
  * */


class IPolyhedronFittingAlgorithm
{
public:
    virtual void fit(Polyhedron& poly, math::vec3 centre,
                     std::vector<Point>* points) = 0;
    virtual ~IPolyhedronFittingAlgorithm(){}
};


#endif // IFITTINGALGORITHM_H
