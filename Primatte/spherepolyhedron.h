#pragma once
#include "matrixd.h"
#include <vector>


//Should replace sin/cos/tan with lookup tables with precision n.
#include "application.h"

class SpherePolyhedron
{
public:

    /** Done */
    //phi,theta
    static math::vec2 cartesianToSpherical(const math::vec3& cartesian);

    /** Done */
    static math::vec3 sphericalToCartesian(const math::vec2& spherical);

    /** UNDONE */
    float findDistanceToPolyhedron(const math::vec3& normalisedVector);

    /** Done */
    //Assumes the ray passes through the triangle (important)
    //Finds the distance to the triangle tri along the vector v from the centre.
    float findDistanceToKnownTriangle(const math::vec3& pointOnTriangle,
                                      const math::vec3& normal,
                                      const math::vec3& v);

    //indexed by [iPhi*(mThetaFaces-1)+ iTheta], where iPhi < mPhiFaces,
    //                                                 iTheta < mThetaFaces-1
    //The last two elements are the poles (north, south)
    std::vector<math::vec3> mVertices;
    math::vec3 mCentre;
    unsigned mPhiFaces, mThetaFaces;
    float mPhiAngle, mThetaAngle;
    float mRadius;

    /** Done */
    void constructMesh();

    unsigned mVerticesPhiCount, mVerticesThetaCount;
    math::vec3 getPointAtIndex(int phi, int theta);

public:


    void debugDraw();

    SpherePolyhedron();

    SpherePolyhedron(unsigned phiFaces, unsigned thetaFaces, math::vec3 centre, float radius);
    /**
      * @param normalisedVector The vector from the centre of the sphere that is to be tested.
      * @return The distance to the triangle from the centre in the direction of the vector.
      */
    float findIntersection(const math::vec3 normalisedVector);
};
