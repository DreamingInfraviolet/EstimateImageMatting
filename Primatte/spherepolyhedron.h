#pragma once
#include "matrixd.h"
#include <vector>
#include <QGLViewer/qglviewer.h>

namespace anima
{

    // * There is a potential minor innacuracy in the theta angle.
    //   Debug preview shows strange jumps when finding distance, yet I can't see anything wrong.

    //Should replace sin/cos/tan with lookup tables with precision n.
    //Issues:
    class SpherePolyhedron
    {
    public:

        /** Done */
        //phi,theta
        static math::vec2 cartesianToSpherical(const math::vec3& cartesian);

        /** Done */
        static math::vec3 sphericalToCartesian(const math::vec2& spherical);

        /** Done */
        float findDistanceToPolyhedron(const math::vec3& normalisedVector) const;

        //indexed by [iPhi*(mThetaFaces-1)+ iTheta], where iPhi < mPhiFaces,
        //                                                 iTheta < mThetaFaces-1
        //The last two elements are the poles (north, south)
        math::vec3 mCentre;
        unsigned mPhiFaces, mThetaFaces;
        float mPhiAngle, mThetaAngle;
        float mRadius;

        /** Done */
        void constructMesh();

        unsigned mVerticesPhiCount, mVerticesThetaCount;
        math::vec3 getPointAtIndex(int phi, int theta) const;

        bool mCentreAndRadiusChanged;

    public:

        std::vector<math::vec3> mVertices;

        void debugDraw(math::vec3 colour) const;

        SpherePolyhedron();


        SpherePolyhedron(unsigned phiFaces, unsigned thetaFaces);
        /**
          * @param normalisedVector The vector from the centre of the sphere that is to be tested.
          * @return The distance to the triangle from the centre in the direction of the vector.
          */
        float findIntersection(const math::vec3 normalisedVector) const;

        /** Can only be done once. */
        void setCentreAndRadius(const math::vec3 centre, float radius);

        /** Returns the centre of the object. */
        math::vec3 centre() const { return mCentre; }
    };

}
