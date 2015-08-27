#pragma once
#include "matrixd.h"
#include <vector>
#include <QGLViewer/qglviewer.h>

/**
  * This class constructs, stores and manages a carefully-constructed 3D spherical
  * polyhedron. Here is how it works:
  * Construction:
  * If the top and bottom triangle rings are ignored, then the polyhedron
  * can be represented as a 2D grid of vertices indexed by angles. This makes
  * face lookup really fast. The top/bottom areas can then be treated as edge cases.
  * Thus, first the 2D grid is constructed, and then the two poles are inserted.
  * Adjacent vertices may be found quickly by storing the vertices in a grid order.
  * Triangle inersection:
  * The appropriate quad is first selected. A quad is made from two triangles:
  * the equation of the line is used to determine which trinagle a ray falls into.
  * The triangle is then chosen, and the distance to the plane lying on the triangle is found.
  * One limitation is that the ray origin must be the sphere origin, and the sphere vertices
  * may not be moved as to violate the angle between them and the origin.
  */


namespace anima
{

    //There is a potential minor innacuracy in the theta angle.
    //Debug preview shows strange jumps when finding distance, yet I can't see anything wrong.

    class SpherePolyhedron
    {
    protected:
        //The centre of the sphere.
        math::vec3 mCentre;

        //The number of quads.
        unsigned mPhiFaces, mThetaFaces;

        //The angles taken up by each quads.
        float mPhiAngle, mThetaAngle;

        //The radius.
        float mRadius;

        //The number of vertices in each direction, excluding the poles.
        unsigned mVerticesPhiCount, mVerticesThetaCount;

        /** Constructs the mesh according to the internal parameters without
          * scaling or positioning it. */
        void constructMesh();


        /** Returns the vertex at the index phi,theta of the vertex grid.
          * Can not return poles unless in error. */
        math::vec3 getPointAtIndex(int phi, int theta) const;

    public:

        /**
          * Converts cartesian coordinates to spherical coordinaes.
          * @param cartesian A cartesian coordinate normalised around the origin.
          * @return The spherical coordinates where x = phi and y = theta.
          */
        static math::vec2 cartesianToSpherical(const math::vec3& cartesian);

        /**
          * Converts spherical coordinates to cartesian normalised around the origin.
          * @param spherical The spherical coordinates to be converted.
          * @return The corresponding cartesian coordinates.
          */
        static math::vec3 sphericalToCartesian(const math::vec2& spherical);


        //The inner vertices, made public as both read/write access is required.
        //Vertices must only be moved towards or from the origin.
        std::vector<math::vec3> mVertices;

        /** Draws the polyhedron with the given colour. */
        void debugDraw(math::vec3 colour) const;

        /** Creates an uninitialised polyhedron. */
        SpherePolyhedron();

        /** Constructs and initialises the polyhedron.
          * @param phiFaces the number of faces around the up axis.
          * @param thetaFaces the number of vertical faces. */
        SpherePolyhedron(unsigned phiFaces, unsigned thetaFaces);

        /**
          * Finds and returns the distance of a vector to the polyhedron.
          * @param normalisedVector The vector from the centre of the sphere that is to be tested.
          * @return The distance to the triangle from the centre in the direction of the vector.
          */
        float findDistanceToPolyhedron(const math::vec3& normalisedVector) const;


        /** Transforms the polyhedron towards the given centre and radius.
          * @param centre The new centre.
          * @param radius The new radius. */
        void setCentreAndRadius(const math::vec3 centre, float radius);

        /** Returns the centre of the object. */
        math::vec3 centre() const { return mCentre; }

        float radius() const {return mRadius; }
    };

}
