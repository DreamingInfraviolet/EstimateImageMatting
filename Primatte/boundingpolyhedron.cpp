#include "boundingpolyhedron.h"
#include "polyhedronloader.h"
#include "io.h"
#include <memory>
#include "stdlib.h"
#include <algorithm>
#include "drawhelpers.h"

BoundingPolyhedron::BoundingPolyhedron()
{
}

/** Does basic initialisation. */
//outer centre = centre of points
//inner/middle centre = centre colour.

//This centre nonsense is temporary. In the future the points will be
//segmented based on how close they are to the background colour, with
//each part having its own sphere around it.
bool BoundingPolyhedron::initialise(IBoundingObjectData* dataIn)
{
    mFitted = false;
    if(!dataIn)
        return false;


    BoundingPolyhedronData* data = dynamic_cast<BoundingPolyhedronData*>(dataIn);

    try
    {
        //Load sphere
        Inform("Loading " + data->meshPath);
        SMeshLib::IO::importOBJ(data->meshPath, &mPoly);

        mScaleMultiplier = data->scaleMultiplier;

        if(data->fitter==nullptr)
            return false;
        mFitter = data->fitter;

        Inform("Done");
    }
    catch(const std::exception& e)
    {
        return false;
    }

    return true;
}

void BoundingPolyhedron::debugDraw()
{
    DrawPolyhedron(mPoly, 0,255,0);
}

bool BoundingPolyhedron::fitted()
{
    return mFitted;
}

template<class T>
T clamp(T x, T min, T max)
{
    if(x<min)
        return min;
    if(x>max)
        return max;
    return x;
}

//Adapted from http://www.gamedev.net/topic/552906-closest-point-on-triangle/
math::vec3 closesPointOnTriangle( const math::vec3 *triangle, const math::vec3 &sourcePosition )
{
    math::vec3 edge0 = triangle[1] - triangle[0];
    math::vec3 edge1 = triangle[2] - triangle[0];
    math::vec3 v0 = triangle[0] - sourcePosition;

    float a = math::dot(edge0, edge0 );
    float b = math::dot(edge0, edge1 );
    float c = math::dot(edge1, edge1 );
    float d = math::dot(edge0, v0 );
    float e = math::dot(edge1, v0 );

    float det = a*c - b*b;
    float s = b*e - c*d;
    float t = b*d - a*e;

    if ( s + t < det )
    {
        if ( s < 0.f )
        {
            if ( t < 0.f )
            {
                if ( d < 0.f )
                {
                    s = clamp( -d/a, 0.f, 1.f );
                    t = 0.f;
                }
                else
                {
                    s = 0.f;
                    t = clamp( -e/c, 0.f, 1.f );
                }
            }
            else
            {
                s = 0.f;
                t = clamp( -e/c, 0.f, 1.f );
            }
        }
        else if ( t < 0.f )
        {
            s = clamp( -d/a, 0.f, 1.f );
            t = 0.f;
        }
        else
        {
            float invDet = 1.f / det;
            s *= invDet;
            t *= invDet;
        }
    }
    else
    {
        if ( s < 0.f )
        {
            float tmp0 = b+d;
            float tmp1 = c+e;
            if ( tmp1 > tmp0 )
            {
                float numer = tmp1 - tmp0;
                float denom = a-2*b+c;
                s = clamp( numer/denom, 0.f, 1.f );
                t = 1-s;
            }
            else
            {
                t = clamp( -e/c, 0.f, 1.f );
                s = 0.f;
            }
        }
        else if ( t < 0.f )
        {
            if ( a+d > b+e )
            {
                float numer = c+e-b-d;
                float denom = a-2*b+c;
                s = clamp( numer/denom, 0.f, 1.f );
                t = 1-s;
            }
            else
            {
                s = clamp( -e/c, 0.f, 1.f );
                t = 0.f;
            }
        }
        else
        {
            float numer = c+e-b-d;
            float denom = a-2*b+c;
            s = clamp( numer/denom, 0.f, 1.f );
            t = 1.f - s;
        }
    }

    return triangle[0] + edge0*s + edge1*t;
}



////plane:vec4(x,y,z,w) = x+y+z=w
//float findSignedDistanceFromPlane(math::vec4 plane, math::vec3 point)
//{
//    math::vec3 pointOnPlane (0, 0, plane.w/plane.z);
//    math::vec3 planeToPoint = point - pointOnPlane;
//    math::vec3 normal = plane.xyz();

//    return planeToPoint.proj(normal);
//}

////Used for the third algorithm attempt
//float BoundingSphere::findTriangleMovementDelta(math::vec3 v1, math::vec3 v2, math::vec3 v3)
//{
//    const float tolerance = 0.05f;
//    const float step = 0.05f;

//    //Find smallest point according to signed distance

//    //plane lying on the triangle
//    math::vec3 normal = math::cross(v2-v1, v1-v3);
//    float d = normal.x*v1.x + normal.y*v1.y + normal.z*v1.z;
//    math::vec4 plane(normal, d);


//    auto minimum= *std::min_element(mPoints->begin(), mPoints->end(),
//    [&] (const Point& p1, const Point& p2)
//    {
//         return findSignedDistanceFromPlane(plane, math::vec3(p1.x(),p1.y(),p1.z())) <
//                                    findSignedDistanceFromPlane(plane, math::vec3(p2.x(),p2.y(),p2.z()));
//    });



//    float minDistance = findSignedDistanceFromPlane(plane, math::vec3(minimum.x(), minimum.y(), minimum.z()));

//    if(minDistance<tolerance)
//        return 0;
//    else
//        return std::min(minDistance, step);
//    }


bool BoundingPolyhedron::fit(std::vector<Point>* points)
{


    //Find bounding sphere
    std::unique_ptr<Sphere> bounding (FindBoundingSphere(*points, 0.01));
    if(!bounding.get())
        return false;

    mCentre = math::vec3(bounding->center().x(),
                         bounding->center().y(),
                         bounding->center().z());

    Real multiplier = sqrt(bounding->squared_radius())*2*mScaleMultiplier;

    //Scale mesh vertices
    for(auto it = mPoly.vertices_begin(); it!=mPoly.vertices_end(); ++it)
    {
        Point& p = it->point();
        p = Point
            (
                p.x()*multiplier+bounding->center().x(),
                p.y()*multiplier+bounding->center().y(),
                p.z()*multiplier+bounding->center().z()
            );
    }

    mFitter->fit(mPoly, mCentre, points);
///////////////////////////////////////////////////////////////////////////


    //Moves each vertex as close as it can without touching any of the points.
    //Result: Nice, but the faces intersect. Should run only once.



    //    //Use smarter plane-based method
    //    static bool done = false;
    //    if(done)
    //        return true;

    //    bool atLeastOneMoved = false;

    //    for ( auto f = mSphere.facets_begin(); f != mSphere.facets_end(); ++f)
    //    {
    //        auto fFaceIterator = f->facet_begin();
    //        Point& a = (fFaceIterator)->vertex()->point();
    //        Point& b = (++fFaceIterator)->vertex()->point();
    //        Point& c = (++fFaceIterator)->vertex()->point();

    //        math::vec3 triangle [3];
    //        triangle[0] = math::vec3 (a.x(),a.y(),a.z());
    //        triangle[1] = math::vec3 (b.x(),b.y(),b.z());
    //        triangle[2] = math::vec3 (c.x(),c.y(),c.z());

    //        float delta = findTriangleMovementDelta(triangle[0], triangle[1], triangle[2]);
    //        if(delta!=0)
    //        {
    //            atLeastOneMoved = true;

    //            math::vec3 vec1 = (mCentre - triangle[0]).normalize()*delta;
    //            math::vec3 vec2 = (mCentre - triangle[1]).normalize()*delta;
    //            math::vec3 vec3 = (mCentre - triangle[2]).normalize()*delta;

    //            a = Point(
    //                    a.x() + vec1.x,
    //                    a.y() + vec1.y,
    //                    a.z() + vec1.z);

    //            b = Point(
    //                    b.x() + vec2.x,
    //                    b.y() + vec2.y,
    //                    b.z() + vec2.z);

    //            c = Point(
    //                    c.x() + vec3.x,
    //                    c.y() + vec3.y,
    //                    c.z() + vec3.z);
    //        }

    //    }

    //    done = false;
    //    return done;





    //    //Move each face according to minimum distance to a point
    //    const int runsDesired = 1;
    //    static int runs = 0;
    //    if(runs>=runsDesired)
    //        return true;

    //    for ( auto f = mSphere.facets_begin(); f != mSphere.facets_end(); ++f)
    //    {
    //        auto fFaceIterator = f->facet_begin();
    //        auto vertex1 = (fFaceIterator)->vertex();
    //        auto vertex2 = (++fFaceIterator)->vertex();
    //        auto vertex3 = (++fFaceIterator)->vertex();

    //        Point& a = vertex1->point();
    //        Point& b = vertex2->point();
    //        Point& c = vertex3->point();

    //        math::vec3 v [3];
    //        v[0] = math::vec3 (a.x(),a.y(),a.z());
    //        v[1] = math::vec3 (b.x(),b.y(),b.z());
    //        v[2] = math::vec3 (c.x(),c.y(),c.z());

    //        //Find closest point the dumb way
    //        math::vec3 point(mPoints->begin()->x(), mPoints->begin()->y(),mPoints->begin()->z());
    //        float closestDistanceSquared =
    //                point.distanceSquared(closesPointOnTriangle(v, point));

    //        for(auto it = mPoints->begin()+1; it!=mPoints->end(); ++it)
    //        {
    //            math::vec3 point(it->x(), it->y(), it->z());

    //            float newDistance = point.distanceSquared(closesPointOnTriangle(v, point));

    //            if(newDistance < closestDistanceSquared)
    //                closestDistanceSquared = newDistance;
    //        }

    //        //

    //        float closestPointDistance = sqrt(closestDistanceSquared);

    //        math::vec3 vec1 = (mCentre - v[0]).normalize()*closestPointDistance*1;
    //        math::vec3 vec2 = (mCentre - v[1]).normalize()*closestPointDistance*1;
    //        math::vec3 vec3 = (mCentre - v[2]).normalize()*closestPointDistance*1;


    //        a = Point
    //            (
    //                a.x() + vec1.x,
    //                a.y() + vec1.y,
    //                a.z() + vec1.z
    //            );

    //        b = Point
    //            (
    //                b.x() + vec2.x,
    //                b.y() + vec2.y,
    //                b.z() + vec2.z
    //            );

    //        c = Point
    //            (
    //                c.x() + vec3.x,
    //                c.y() + vec3.y,
    //                c.z() + vec3.z
    //            );
    //    }



    //    ++runs;
    //    return false;

}

//maps x in range[r1min,r1max] to [r2min,r1max].
float mapRange(float x, float r1min, float r1max, float r2min, float r2max)
{
    return (x + r2min - r1min) * ((r1max-r1min)/(r2max-r2min));
}

//float BoundingSphere::findPixelAlpha(IBoundingObjectPixel px, IBoundingObjectPixel backgroundPx)
//{
//    //Distance based alpha:
//    float distanceFullAlpha = 0.4f, distanceNoAlpha = 0.2f;
//    float a = (float)px.r()- backgroundPx.r();
//    float b = (float)px.g()- backgroundPx.g();
//    float c = (float)px.b()- backgroundPx.b();
//    float distance = sqrt(a*a+b*b+c*c)/255.f;

//    float ans = (distance - distanceNoAlpha) / (distanceFullAlpha - distanceNoAlpha);
//    ans = ans<0 ? 0:ans;
//    ans = ans>1 ? 1:ans;
//    return ans;

//    return 0.0f;
//}
