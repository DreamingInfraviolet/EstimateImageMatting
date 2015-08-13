#include "matrixd.h"
#include <cmath>

namespace math
{
    template<class T> T dot(const tmp::_vec2<T>& v1, const tmp::_vec2<T>& v2)
    {
        return v1.x*v2.x + v1.y*v2.y;
    }

    template<class T> T dot(const tmp::_vec3<T>& v1, const tmp::_vec3<T>& v2)
    {
        return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
    }

    template<class T> T dot(const tmp::_vec4<T>& v1, const tmp::_vec4<T>& v2)
    {
        return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
    }

    template<class T> tmp::_vec3<T> cross(const tmp::_vec3<T>& v1, const tmp::_vec3<T>& v2)
    {
        return tmp::_vec3<T>(v1.y*v2.z - v1.z*v2.y,
            v1.z*v2.x - v1.x*v2.z,
            v1.x*v2.y - v1.y*v2.x);
    }

    namespace tmp
    {
        static const matReal matScalarOne(1);
        static const matReal matScalarNull(0);
        static const matReal matScalarTwo(2);

        template<class T> _vec2<T>::_vec2()
        {
            reset();
        }

        template<class T> _vec2<T>::_vec2(const T x_, const T y_)
        {
            x = x_;
            y = y_;
        }

        template<class T> _vec2<T>::_vec2(const T n)
        {
            x = n;
            y = n;
        }

        template<class T> matReal _vec2<T>::angle() const
        {
            return matReal(std::atan2(y, x));
        }

        template<class T> T _vec2<T>::lengthSquared() const
        {
            return x*x + y*y;
        }

        template<class T> matReal _vec2<T>::length() const
        {
            return matReal(std::sqrt(x*x + y*y));
        }

        template<class T> matReal _vec2<T>::proj(const _vec2<T>& v) const
        {
             return math::dot(*this, v/v.length());
        }

        template<class T> matReal _vec2<T>::distance(const _vec2<T>& v) const
        {
            return matReal(std::sqrt((x - v.x)*(x - v.x) + (y - v.y)*(y - v.y)));
        }

        template<class T> T _vec2<T>::distanceSquared(const _vec2<T>& v) const
        {
            return (x - v.x)*(x - v.x) + (y - v.y)*(y - v.y);
        }

        template<class T> _vec2<T> _vec2<T>::operator * (const T n) const
        {
            return _vec2<T>(x*n, y*n);
        }

        template<class T> _vec2<T> _vec2<T>::operator / (const T n) const
        {
            return _vec2<T>(x / n, y / n);
        }

        template<class T> _vec2<T> _vec2<T>::operator + (const T n) const
        {
            return _vec2<T>(x + n, y + n);
        }

        template<class T> _vec2<T> _vec2<T>::operator - (const T n) const
        {
            return _vec2<T>(x - n, y - n);
        }

        template<class T> void  _vec2<T>::operator *= (const T n)
        {
            x *= n;
            y *= n;
        }

        template<class T> void _vec2<T>::operator /= (const T n)
        {
            x /= n;
            y /= n;
        }

        template<class T> void _vec2<T>::operator += (const T n)
        {
            x += n;
            y += n;
        }

        template<class T> void _vec2<T>::operator -= (const T n)
        {
            x -= n;
            y -= n;
        }

        template<class T> bool _vec2<T>::operator == (const _vec2<T>& vec) const
        {
            return x == vec.x && y == vec.y;
        }

        template<class T> bool _vec2<T>::operator == (T f) const
        {
            return x == f && y == f;
        }

        template<class T> bool _vec2<T>::operator != (const _vec2<T>& vec) const
        {
            return x != vec.x || y != vec.y;
        }

        template<class T> bool _vec2<T>::operator != (T f) const
        {
            return x != f || y != f;
        }

        template<class T> _vec2<T>& _vec2<T>::normalize()
        {
            const matReal len = length();
            if(len==0)
                *this = _vec2<T>(0);
            else
            {
                const matReal lenDivisor = matScalarOne / len;
                x = T(x * lenDivisor);
                y = T(y * lenDivisor);
            }
            return *this;
        }

        template<class T> _vec2<T> _vec2<T>::operator + (const _vec2<T>& vec) const
        {
            return _vec2<T>(x + vec.x, y + vec.y);
        }

        template<class T> _vec2<T> _vec2<T>::operator - (const _vec2<T>& vec) const
        {
            return _vec2<T>(x - vec.x, y - vec.y);
        }

        template<class T> _vec2<T> _vec2<T>::operator * (const _vec2<T>& vec) const
        {
            return _vec2<T>(x*vec.x, y*vec.y);
        }

        template<class T> _vec2<T> _vec2<T>::operator / (const _vec2<T>& vec) const
        {
            return _vec2<T>(x / vec.x, y / vec.y);
        }

        template<class T> void _vec2<T>::operator += (const _vec2<T>& vec)
        {
            x += vec.x;
            y += vec.y;
        }

        template<class T> void _vec2<T>::operator -= (const _vec2<T>& vec)
        {
            x -= vec.x;
            y -= vec.y;
        }

        template<class T> void _vec2<T>::operator *= (const _vec2<T>& vec)
        {
            x *= vec.x;
            y *= vec.y;
        }

        template<class T> void _vec2<T>::operator /= (const _vec2<T>& vec)
        {
            x /= vec.x;
            y /= vec.y;
        }

        template<class T> _vec2<T> _vec2<T>::xy() const { return _vec2<T>(x, y); }
        template<class T> _vec2<T> _vec2<T>::yx() const { return _vec2<T>(y, x); }

        template<class T> void _vec2<T>::reset()
        {
            x = 0;
            y = 0;
        }

        template<class T> bool _vec2<T>::operator < (const _vec2<T>& vec) const
        {
            if (x==vec.x)
                return y<vec.y;
            else
                return x < vec.x;
        }

        template<class T> bool _vec2<T>::operator <= (const _vec2<T>& vec) const
        {
            return *this < vec || *this==vec;
        }

        template<class T> bool _vec2<T>::operator > (const _vec2<T>& vec) const
        {
            if (x==vec.x)
                return y>vec.y;
            else
                return x > vec.x;
        }

        template<class T> bool _vec2<T>::operator >= (const _vec2<T>& vec) const
        {
            return *this > vec || *this==vec;
        }

        template<class T> _vec2<T> _vec2<T>::operator % (const T n) const
        {
            return _vec2<T>(x%n, y%n);
        }

        template<class T> void _vec2<T>::operator %= (const T n)
        {
            x%=n;
            y%=n;
        }

        template<class T> _vec2<T> _vec2<T>::operator % (const _vec2<T>& vec) const
        {
            return _vec2<T>(x%vec.x, y%vec.y);
        }

        template<class T> void _vec2<T>::operator %= (const _vec2<T>& vec)
        {
            x%=vec.x;
            y%=vec.y;
        }


        template<> _vec2<float> _vec2<float>::operator % (const float n) const
        {
            return _vec2<float>((float)fmod(x,n),(float)fmod(y,n));
        }

        template<> void _vec2<float>::operator %= (const float n)
        {
            x = (float)fmod(x,n);
            y = (float)fmod(y,n);
        }

        template<> _vec2<float> _vec2<float>::operator % (const _vec2<float>& vec) const
        {
            return _vec2<float>((float)fmod(x,vec.x), (float)fmod(y,vec.y));
        }

        template<> void _vec2<float>::operator %= (const _vec2<float>& vec)
        {
            x = (float)fmod(x,vec.x);
            y = (float)fmod(y,vec.y);
        }


        template<> _vec2<double> _vec2<double>::operator % (const double n) const
        {
            return _vec2<double>((double)fmod(x,n),(double)fmod(y,n));
        }

        template<> void _vec2<double>::operator %= (const double n)
        {
            x = (double)fmod(x,n);
            y = (double)fmod(y,n);
        }

        template<> _vec2<double> _vec2<double>::operator % (const _vec2<double>& vec) const
        {
            return _vec2<double>((double)fmod(x,vec.x), (double)fmod(y,vec.y));
        }

        template<> void _vec2<double>::operator %= (const _vec2<double>& vec)
        {
            x = (double)fmod(x,vec.x);
            y = (double)fmod(y,vec.y);
        }


        template<class T> _vec3<T>::_vec3()
        {
            reset();
        }

        template<class T> _vec3<T>::_vec3(const T x_, const T y_, const T z_)
        {
            x = x_;
            y = y_;
            z = z_;
        }

        template<class T> _vec3<T>::_vec3(const _vec2<T>& v, const T f)
        {
            x = v.x;
            y = v.y;
            z = f;
        }

        template<class T> _vec3<T>::_vec3(const T f, const _vec2<T>& v)
        {
            x = f;
            y = v.x;
            z = v.y;
        }

        template<class T> _vec3<T>::_vec3(const T f)
        {
            x = f;
            y = f;
            z = f;
        }

        template<class T> T _vec3<T>::lengthSquared() const
        {
            return x*x + y*y + z*z;
        }

        template<class T> matReal _vec3<T>::length() const
        {
            return matReal(std::sqrt(x*x + y*y + z*z));
        }

        template<class T> matReal _vec3<T>::distance(const _vec3<T>& v) const
        {
            return matReal(std::sqrt((x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z)));
        }

        template<class T> T _vec3<T>::distanceSquared(const _vec3<T>& v) const
        {
            return (x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z);
        }

        template<class T> matReal _vec3<T>::proj(const _vec3<T>& v) const
        {
             return math::dot(*this, v/v.length());
        }

        template<class T> _vec3<T> _vec3<T>::operator * (const T n) const
        {
            return _vec3<T>(x*n, y*n, z*n);
        }

        template<class T> _vec3<T> _vec3<T>::operator / (const T n) const
        {
            return _vec3<T>(x / n, y / n, z / n);
        }

        template<class T> _vec3<T> _vec3<T>::operator + (const T n) const
        {
            return _vec3<T>(x + n, y + n, z + n);
        }

        template<class T> _vec3<T> _vec3<T>::operator - (const T n) const
        {
            return _vec3<T>(x - n, y - n, z - n);
        }

        template<class T> void _vec3<T>::operator *= (const T n)
        {
            x *= n;
            y *= n;
            z *= n;
        }

        template<class T> void _vec3<T>::operator /= (const T n)
        {
            x /= n;
            y /= n;
            z /= n;
        }

        template<class T> void _vec3<T>::operator += (const T n)
        {
            x += n;
            y += n;
            z += n;
        }

        template<class T> void _vec3<T>::operator -= (const T n)
        {
            x -= n;
            y -= n;
            z -= n;
        }

        template<class T>
        bool _vec3<T>::operator == (const _vec3<T>& vec) const
        {
            return x == vec.x && y == vec.y && z == vec.z;
        }

        template<class T> bool _vec3<T>::operator == (T f) const
        {
            return x == f && y == f && z == f;
        }

        template<class T> bool _vec3<T>::operator != (const _vec3<T>& vec) const
        {
            return x != vec.x || y != vec.y || z != vec.z;
        }

        template<class T> bool _vec3<T>::operator != (T f) const
        {
            return x != f || y != f || z != f;
        }

        template<class T> _vec3<T>& _vec3<T>::normalize()
        {
            const matReal len = length();
            if(len==0)
                *this = _vec3<T>(T(0));
            else
            {
                const matReal lenDivisor = matScalarOne / len;
                x = T(x * lenDivisor);
                y = T(y * lenDivisor);
                z = T(z * lenDivisor);
            }
            return *this;
        }

        template<class T> _vec3<T> _vec3<T>::operator + (const _vec3<T>& vec) const
        {
            return _vec3<T>(x + vec.x, y + vec.y, z + vec.z);
        }

        template<class T> _vec3<T> _vec3<T>::operator - (const _vec3<T>& vec) const
        {
            return _vec3<T>(x - vec.x, y - vec.y, z - vec.z);
        }

        template<class T> _vec3<T> _vec3<T>::operator * (const _vec3<T>& vec) const
        {
            return _vec3<T>(x*vec.x, y*vec.y, z*vec.z);
        }

        template<class T> _vec3<T> _vec3<T>::operator / (const _vec3<T>& vec) const
        {
            return _vec3<T>(x / vec.x, y / vec.y, z / vec.z);
        }

        template<class T> void _vec3<T>::operator += (const _vec3<T>& vec)
        {
            x += vec.x;
            y += vec.y;
            z += vec.z;
        }

        template<class T> void _vec3<T>::operator -= (const _vec3<T>& vec)
        {
            x -= vec.x;
            y -= vec.y;
            z -= vec.z;
        }

        template<class T> void _vec3<T>::operator *= (const _vec3<T>& vec)
        {
            x *= vec.x;
            y *= vec.y;
            z *= vec.z;
        }

        template<class T> void _vec3<T>::operator /= (const _vec3<T>& vec)
        {
            x /= vec.x;
            y /= vec.y;
            z /= vec.z;
        }

        template<class T> _vec3<T>& _vec3<T>::rotate(const matReal angle, const _vec3<matReal>& axis)
        {
            mat4 rotationmat;
            rotationmat.initRotation(angle, axis);

            *this = (rotationmat*_vec4<T>(*this, 1)).xyz();
            return *this;
        }

        template<class T> void _vec3<T>::reset()
        {
            x = 0;
            y = 0;
            z = 0;
        }

        template<class T> _vec3<T> _vec3<T>::xyz() const { return _vec3<T>(x, y, z); }
        template<class T> _vec3<T> _vec3<T>::xzy() const { return _vec3<T>(x, z, y); }
        template<class T> _vec3<T> _vec3<T>::yxz() const { return _vec3<T>(y, x, z); }
        template<class T> _vec3<T> _vec3<T>::yzx() const { return _vec3<T>(y, z, x); }
        template<class T> _vec3<T> _vec3<T>::zxy() const { return _vec3<T>(z, x, y); }
        template<class T> _vec3<T> _vec3<T>::zyx() const { return _vec3<T>(z, y, x); }

        template<class T> _vec2<T> _vec3<T>::xy() const { return _vec2<T>(x, y); }
        template<class T> _vec2<T> _vec3<T>::yx() const { return _vec2<T>(y, x); }
        template<class T> _vec2<T> _vec3<T>::xz() const { return _vec2<T>(x, z); }
        template<class T> _vec2<T> _vec3<T>::zx() const { return _vec2<T>(z, x); }
        template<class T> _vec2<T> _vec3<T>::yz() const { return _vec2<T>(y, z); }
        template<class T> _vec2<T> _vec3<T>::zy() const { return _vec2<T>(z, y); }


        template<class T> bool _vec3<T>::operator < (const _vec3<T>& vec) const
        {
            if (x==vec.x)
                if(y==vec.y)
                    return z<vec.z;
                else
                    return y<vec.y;
            else
                return x < vec.x;
        }

        template<class T> bool _vec3<T>::operator <= (const _vec3<T>& vec) const
        {
            return *this < vec || *this==vec;
        }

        template<class T> bool _vec3<T>::operator > (const _vec3<T>& vec) const
        {
            if (x==vec.x)
                if(y==vec.y)
                    return z>vec.z;
                else
                    return y>vec.y;
            else
                return x > vec.x;
        }

        template<class T> bool _vec3<T>::operator >= (const _vec3<T>& vec) const
        {
            return *this > vec || *this==vec;
        }

        template<class T> _vec3<T> _vec3<T>::operator % (const T n) const
        {
            return _vec3<T>(x%n, y%n, z%n);
        }

        template<class T> void _vec3<T>::operator %= (const T n)
        {
            x%=n;
            y%=n;
            z%=n;
        }

        template<class T> _vec3<T> _vec3<T>::operator % (const _vec3<T>& vec) const
        {
            return _vec3<T>(x%vec.x, y%vec.y, z%vec.z);
        }

        template<class T> void _vec3<T>::operator %= (const _vec3<T>& vec)
        {
            x%=vec.x;
            y%=vec.y;
            z%=vec.z;
        }


        template<> _vec3<float> _vec3<float>::operator % (const float n) const
        {
            return _vec3<float>((float)fmod(x,n),(float)fmod(y,n),(float)fmod(z,n));
        }

        template<> void _vec3<float>::operator %= (const float n)
        {
            x = (float)fmod(x,n);
            y = (float)fmod(y,n);
            z = (float)fmod(z,n);
        }

        template<> _vec3<float> _vec3<float>::operator % (const _vec3<float>& vec) const
        {
            return _vec3<float>((float)fmod(x,vec.x), (float)fmod(y,vec.y), (float)fmod(z,vec.z));
        }

        template<> void _vec3<float>::operator %= (const _vec3<float>& vec)
        {
            x = (float)fmod(x,vec.x);
            y = (float)fmod(y,vec.y);
            z = (float)fmod(z,vec.z);
        }


        template<> _vec3<double> _vec3<double>::operator % (const double n) const
        {
            return _vec3<double>((double)fmod(x,n),(double)fmod(y,n),(double)fmod(z,n));
        }

        template<> void _vec3<double>::operator %= (const double n)
        {
            x = (double)fmod(x,n);
            y = (double)fmod(y,n);
            z = (double)fmod(z,n);
        }

        template<> _vec3<double> _vec3<double>::operator % (const _vec3<double>& vec) const
        {
            return _vec3<double>((double)fmod(x,vec.x), (double)fmod(y,vec.y), (double)fmod(z,vec.z));
        }

        template<> void _vec3<double>::operator %= (const _vec3<double>& vec)
        {
            x = (double)fmod(x,vec.x);
            y = (double)fmod(y,vec.y);
            z = (double)fmod(z,vec.z);
        }





        template<class T> _vec4<T>::_vec4()
        {
            reset();
        }

        template<class T> _vec4<T>::_vec4(const T x_, const T y_, const T z_, const T w_)
        {
            x = x_;
            y = y_;
            z = z_;
            w = w_;
        }

        template<class T> _vec4<T>::_vec4(const _vec2<T>& v1, const _vec2<T>& v2)
        {
            x = v1.x;
            y = v1.y;
            z = v2.x;
            w = v2.y;
        }

        template<class T> _vec4<T>::_vec4(const T f1, const T f2, const _vec2<T>& v)
        {
            x = f1;
            y = f2;
            z = v.x;
            w = v.y;
        }

        template<class T> _vec4<T>::_vec4(const _vec2<T>& v, const T f1, const T f2)
        {
            x = v.x;
            y = v.y;
            z = f1;
            w = f2;
        }

        template<class T> _vec4<T>::_vec4(const T f1, const _vec2<T>& v, const T f2)
        {
            x = f1;
            y = v.x;
            z = v.y;
            w = f2;
        }

        template<class T> _vec4<T>::_vec4(const _vec3<T>& v, const T f)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = f;
        }

        template<class T> _vec4<T>::_vec4(const T f, const _vec3<T>& v)
        {
            x = f;
            y = v.x;
            z = v.y;
            w = v.z;
        }

        template<class T> _vec4<T>::_vec4(const T f)
        {
            x = f;
            y = f;
            z = f;
            w = f;
        }

        template<class T> T _vec4<T>::lengthSquared() const
        {
            return x*x + y*y + z*z + w*w;
        }

        template<class T> matReal _vec4<T>::length() const
        {
            return matReal(std::sqrt(x*x + y*y + z*z + w*w));
        }

        template<class T>
        matReal _vec4<T>::distance(const _vec4<T>& v) const
        {
            return matReal(sqrt((x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z) + (w - v.w)*(w - v.w)));
        }

        template<class T> T _vec4<T>::distanceSquared(const _vec4<T>& v) const
        {
            return (x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z) + (w - v.w)*(w - v.w);
        }

        template<class T> matReal _vec4<T>::proj(const _vec4<T>& v)  const
        {
             return math::dot(*this, v/v.length());
        }

        template<class T> _vec4<T> _vec4<T>::operator * (const T n) const
        {
            return _vec4<T>(x*n, y*n, z*n, w*n);
        }

        template<class T> _vec4<T> _vec4<T>::operator / (const T n) const
        {
            return _vec4<T>(x / n, y / n, z / n, w / n);
        }

        template<class T> _vec4<T> _vec4<T>::operator + (const T n) const
        {
            return _vec4<T>(x + n, y + n, z + n, w + n);
        }

        template<class T> _vec4<T> _vec4<T>::operator - (const T n) const
        {
            return _vec4<T>(x - n, y - n, z - n, w - n);
        }

        template<class T> void _vec4<T>::operator *= (const T n)
        {
            x *= n;
            y *= n;
            z *= n;
            w *= n;
        }

        template<class T> void  _vec4<T>::operator /= (const T n)
        {
            x /= n;
            y /= n;
            z /= n;
            w /= n;
        }

        template<class T> void  _vec4<T>::operator += (const T n)
        {
            x += n;
            y += n;
            z += n;
            w += n;
        }

        template<class T> void  _vec4<T>::operator -= (const T n)
        {
            x -= n;
            y -= n;
            z -= n;
            w -= n;
        }

        template<class T> bool _vec4<T>::operator == (const _vec4<T>& vec) const
        {
            return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
        }

        template<class T> bool _vec4<T>::operator == (T f) const
        {
            return x == f && y == f &&z == f && w == f;
        }

        template<class T> bool _vec4<T>::operator != (const _vec4<T>& vec) const
        {
            return x != vec.x || y != vec.y || z != vec.z || w != vec.w;
        }

        template<class T> bool _vec4<T>::operator != (T f) const
        {
            return x != f || y != f || z != f || w != f;
        }

        template<class T> _vec4<T>& _vec4<T>::normalize()
        {
            const matReal len = length();
            if(len==0)
                *this = _vec4<T>(0);
            else
            {
                const matReal lenDivisor = matScalarOne / len;
                x = T(x * lenDivisor);
                y = T(y * lenDivisor);
                z = T(z * lenDivisor);
                w = T(w * lenDivisor);
            }
            return *this;
        }

        template<class T> _vec4<T> _vec4<T>::operator + (const _vec4<T>& vec) const
        {
            return _vec4<T>(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
        }

        template<class T> _vec4<T> _vec4<T>::operator - (const _vec4<T>& vec) const
        {
            return _vec4<T>(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
        }

        template<class T>
        _vec4<T> _vec4<T>::operator * (const _vec4<T>& vec) const
        {
            return _vec4<T>(x*vec.x, y*vec.y, z*vec.z, w*vec.w);
        }

        template<class T> _vec4<T> _vec4<T>::operator / (const _vec4<T>& vec) const
        {
            return _vec4<T>(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
        }

        template<class T> void _vec4<T>::operator += (const _vec4<T>& vec)
        {
            x += vec.x;
            y += vec.y;
            z += vec.z;
            w += vec.w;
        }
        template<class T> void _vec4<T>::operator -= (const _vec4<T>& vec)
        {
            x -= vec.x;
            y -= vec.y;
            z -= vec.z;
            w -= vec.w;
        }

        template<class T> void _vec4<T>::operator *= (const _vec4<T>& vec)
        {
            x *= vec.x;
            y *= vec.y;
            z *= vec.z;
            w *= vec.w;
        }

        template<class T> void _vec4<T>::operator /= (const _vec4<T>& vec)
        {
            x /= vec.x;
            y /= vec.y;
            z /= vec.z;
            w /= vec.w;
        }

        template<class T> void _vec4<T>::reset()
        {
            x = 0;
            y = 0;
            z = 0;
            w = 0;
        }

        template<class T> _vec4<T> _vec4<T>::operator % (const T n) const
        {
            return _vec4<T>(x%n, y%n, z%n, w%n);
        }

        template<class T> void _vec4<T>::operator %= (const T n)
        {
            x%=n;
            y%=n;
            z%=n;
            w%=n;
        }

        template<class T> _vec4<T> _vec4<T>::operator % (const _vec4<T>& vec) const
        {
            return _vec4<T>(x%vec.x, y%vec.y, z%vec.z, w%vec.w);
        }

        template<class T> void _vec4<T>::operator %= (const _vec4<T>& vec)
        {
            x%=vec.x;
            y%=vec.y;
            z%=vec.z;
            w%=vec.w;
        }



        template<> _vec4<float> _vec4<float>::operator % (const float n) const
        {
            return _vec4<float>((float)fmod(x,n),(float)fmod(y,n),(float)fmod(z,n),(float)fmod(w,n));
        }

        template<> void _vec4<float>::operator %= (const float n)
        {
            x = (float)fmod(x,n);
            y = (float)fmod(y,n);
            z = (float)fmod(z,n);
            w = (float)fmod(w,n);
        }

        template<> _vec4<float> _vec4<float>::operator % (const _vec4<float>& vec) const
        {
            return _vec4<float>((float)fmod(x,vec.x), (float)fmod(y,vec.y), (float)fmod(z,vec.z),(float)fmod(w,vec.w));
        }

        template<> void _vec4<float>::operator %= (const _vec4<float>& vec)
        {
            x = (float)fmod(x,vec.x);
            y = (float)fmod(y,vec.y);
            z = (float)fmod(z,vec.z);
            w = (float)fmod(w,vec.w);
        }


        template<> _vec4<double> _vec4<double>::operator % (const double n) const
        {
            return _vec4<double>((double)fmod(x,n),(double)fmod(y,n),(double)fmod(z,n),(double)fmod(w,n));
        }

        template<> void _vec4<double>::operator %= (const double n)
        {
            x = (double)fmod(x,n);
            y = (double)fmod(y,n);
            z = (double)fmod(z,n);
            w = (double)fmod(w,n);
        }

        template<> _vec4<double> _vec4<double>::operator % (const _vec4<double>& vec) const
        {
            return _vec4<double>((double)fmod(x,vec.x), (double)fmod(y,vec.y), (double)fmod(z,vec.z),(double)fmod(w,vec.w));
        }

        template<> void _vec4<double>::operator %= (const _vec4<double>& vec)
        {
            x = (double)fmod(x,vec.x);
            y = (double)fmod(y,vec.y);
            z = (double)fmod(z,vec.z);
            w = (double)fmod(w,vec.w);
        }





        template<class T> _vec4<T> _vec4<T>::xyzw() const { return _vec4<T>(x, y, z, w); }
        template<class T> _vec4<T> _vec4<T>::xywz() const { return _vec4<T>(x, y, w, z); }
        template<class T> _vec4<T> _vec4<T>::xzwy() const { return _vec4<T>(x, z, w, y); }
        template<class T> _vec4<T> _vec4<T>::xzyw() const { return _vec4<T>(x, z, y, w); }
        template<class T> _vec4<T> _vec4<T>::xwzy() const { return _vec4<T>(x, w, z, y); }
        template<class T> _vec4<T> _vec4<T>::xwyz() const { return _vec4<T>(x, w, y, z); }
        template<class T> _vec4<T> _vec4<T>::yxzw() const { return _vec4<T>(y, x, z, w); }
        template<class T> _vec4<T> _vec4<T>::yxwz() const { return _vec4<T>(y, x, w, z); }
        template<class T> _vec4<T> _vec4<T>::yzwx() const { return _vec4<T>(y, z, w, x); }
        template<class T> _vec4<T> _vec4<T>::yzxw() const { return _vec4<T>(y, z, x, w); }
        template<class T> _vec4<T> _vec4<T>::ywzx() const { return _vec4<T>(y, w, z, x); }
        template<class T> _vec4<T> _vec4<T>::ywxz() const { return _vec4<T>(y, w, x, z); }
        template<class T> _vec4<T> _vec4<T>::zyxw() const { return _vec4<T>(z, y, x, w); }
        template<class T> _vec4<T> _vec4<T>::zywx() const { return _vec4<T>(z, y, w, x); }
        template<class T> _vec4<T> _vec4<T>::zxwy() const { return _vec4<T>(z, x, w, y); }
        template<class T> _vec4<T> _vec4<T>::zxyw() const { return _vec4<T>(z, x, y, w); }
        template<class T> _vec4<T> _vec4<T>::zwxy() const { return _vec4<T>(z, w, x, y); }
        template<class T> _vec4<T> _vec4<T>::zwyx() const { return _vec4<T>(z, w, y, x); }
        template<class T> _vec4<T> _vec4<T>::wyzx() const { return _vec4<T>(w, y, z, x); }
        template<class T> _vec4<T> _vec4<T>::wyxz() const { return _vec4<T>(w, y, x, z); }
        template<class T> _vec4<T> _vec4<T>::wzxy() const { return _vec4<T>(w, z, x, y); }
        template<class T> _vec4<T> _vec4<T>::wzyx() const { return _vec4<T>(w, z, y, x); }
        template<class T> _vec4<T> _vec4<T>::wxzy() const { return _vec4<T>(w, x, z, y); }
        template<class T> _vec4<T> _vec4<T>::wxyz() const { return _vec4<T>(w, x, y, z); }

        template<class T> _vec3<T> _vec4<T>::xyz() const { return _vec3<T>(x, y, z); }
        template<class T> _vec3<T> _vec4<T>::xzy() const { return _vec3<T>(x, z, y); }
        template<class T> _vec3<T> _vec4<T>::yxz() const { return _vec3<T>(y, x, z); }
        template<class T> _vec3<T> _vec4<T>::yzx() const { return _vec3<T>(y, z, x); }
        template<class T> _vec3<T> _vec4<T>::zxy() const { return _vec3<T>(z, x, y); }
        template<class T> _vec3<T> _vec4<T>::zyx() const { return _vec3<T>(z, y, x); }
        template<class T> _vec3<T> _vec4<T>::wyz() const { return _vec3<T>(w, y, z); }
        template<class T> _vec3<T> _vec4<T>::wzy() const { return _vec3<T>(w, z, y); }
        template<class T> _vec3<T> _vec4<T>::ywz() const { return _vec3<T>(y, w, z); }
        template<class T> _vec3<T> _vec4<T>::yzw() const { return _vec3<T>(y, z, w); }
        template<class T> _vec3<T> _vec4<T>::zwy() const { return _vec3<T>(z, w, y); }
        template<class T> _vec3<T> _vec4<T>::zyw() const { return _vec3<T>(z, y, w); }
        template<class T> _vec3<T> _vec4<T>::xyw() const { return _vec3<T>(x, y, w); }
        template<class T> _vec3<T> _vec4<T>::xwy() const { return _vec3<T>(x, w, y); }
        template<class T> _vec3<T> _vec4<T>::yxw() const { return _vec3<T>(y, x, w); }
        template<class T> _vec3<T> _vec4<T>::ywx() const { return _vec3<T>(y, w, x); }
        template<class T> _vec3<T> _vec4<T>::wxy() const { return _vec3<T>(w, x, y); }
        template<class T> _vec3<T> _vec4<T>::wyx() const { return _vec3<T>(w, y, x); }
        template<class T> _vec3<T> _vec4<T>::xwz() const { return _vec3<T>(x, w, z); }
        template<class T> _vec3<T> _vec4<T>::xzw() const { return _vec3<T>(x, z, w); }
        template<class T> _vec3<T> _vec4<T>::wxz() const { return _vec3<T>(w, x, z); }
        template<class T> _vec3<T> _vec4<T>::wzx() const { return _vec3<T>(w, z, x); }
        template<class T> _vec3<T> _vec4<T>::zxw() const { return _vec3<T>(z, x, w); }
        template<class T> _vec3<T> _vec4<T>::zwx() const { return _vec3<T>(z, w, x); }

        template<class T> _vec2<T> _vec4<T>::xy() const { return _vec2<T>(x, y); }
        template<class T> _vec2<T> _vec4<T>::yx() const { return _vec2<T>(y, x); }
        template<class T> _vec2<T> _vec4<T>::xz() const { return _vec2<T>(x, z); }
        template<class T> _vec2<T> _vec4<T>::zx() const { return _vec2<T>(z, x); }
        template<class T> _vec2<T> _vec4<T>::xw() const { return _vec2<T>(x, w); }
        template<class T> _vec2<T> _vec4<T>::wx() const { return _vec2<T>(w, x); }
        template<class T> _vec2<T> _vec4<T>::yz() const { return _vec2<T>(y, z); }
        template<class T> _vec2<T> _vec4<T>::zy() const { return _vec2<T>(z, y); }
        template<class T> _vec2<T> _vec4<T>::wz() const { return _vec2<T>(w, z); }
        template<class T> _vec2<T> _vec4<T>::zw() const { return _vec2<T>(z, w); }


        template<class T> bool _vec4<T>::operator < (const _vec4<T>& vec) const
        {
            if (x==vec.x)
                if(y==vec.y)
                    if(z==vec.z)
                        return w<vec.w;
                    else
                        return z<vec.z;
                else
                    return y<vec.y;
            else
                return x < vec.x;
        }

        template<class T> bool _vec4<T>::operator <= (const _vec4<T>& vec) const
        {
            return *this < vec || *this==vec;
        }

        template<class T> bool _vec4<T>::operator > (const _vec4<T>& vec) const
        {
            if (x==vec.x)
                if(y==vec.y)
                    if(z==vec.z)
                        return w<vec.w;
                    else
                        return z<vec.z;
                else
                    return y<vec.y;
            else
                return x < vec.x;
        }

        template<class T> bool _vec4<T>::operator >= (const _vec4<T>& vec) const
        {
            return *this > vec || *this==vec;
        }


        _mat4::_mat4()
        {
            reset();
        }

        _mat4::_mat4(const matReal n)
        {
            reset();
            m[0][0] = n; m[1][1] = n;
            m[2][2] = n; m[3][3] = n;
        }

        _mat4::_mat4(const matReal m00, const matReal m10, const matReal m20, const matReal m30,
            const matReal m01, const matReal m11, const matReal m21, const matReal m31,
            const matReal m02, const matReal m12, const matReal m22, const matReal m32,
            const matReal m03, const matReal m13, const matReal m23, const matReal m33)
        {
            m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
            m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
            m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
            m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
        }

        void _mat4::reset()
        {
            m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
            m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
            m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
            m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
        }

        _mat4 _mat4::operator * (const _mat4& p) const
        {
            return _mat4(
                m[0][0] * p.m[0][0] + m[1][0] * p.m[0][1] + m[2][0] * p.m[0][2] + m[3][0] * p.m[0][3],
                m[0][0] * p.m[1][0] + m[1][0] * p.m[1][1] + m[2][0] * p.m[1][2] + m[3][0] * p.m[1][3],
                m[0][0] * p.m[2][0] + m[1][0] * p.m[2][1] + m[2][0] * p.m[2][2] + m[3][0] * p.m[2][3],
                m[0][0] * p.m[3][0] + m[1][0] * p.m[3][1] + m[2][0] * p.m[3][2] + m[3][0] * p.m[3][3],
                m[0][1] * p.m[0][0] + m[1][1] * p.m[0][1] + m[2][1] * p.m[0][2] + m[3][1] * p.m[0][3],
                m[0][1] * p.m[1][0] + m[1][1] * p.m[1][1] + m[2][1] * p.m[1][2] + m[3][1] * p.m[1][3],
                m[0][1] * p.m[2][0] + m[1][1] * p.m[2][1] + m[2][1] * p.m[2][2] + m[3][1] * p.m[2][3],
                m[0][1] * p.m[3][0] + m[1][1] * p.m[3][1] + m[2][1] * p.m[3][2] + m[3][1] * p.m[3][3],
                m[0][2] * p.m[0][0] + m[1][2] * p.m[0][1] + m[2][2] * p.m[0][2] + m[3][2] * p.m[0][3],
                m[0][2] * p.m[1][0] + m[1][2] * p.m[1][1] + m[2][2] * p.m[1][2] + m[3][2] * p.m[1][3],
                m[0][2] * p.m[2][0] + m[1][2] * p.m[2][1] + m[2][2] * p.m[2][2] + m[3][2] * p.m[2][3],
                m[0][2] * p.m[3][0] + m[1][2] * p.m[3][1] + m[2][2] * p.m[3][2] + m[3][2] * p.m[3][3],
                m[0][3] * p.m[0][0] + m[1][3] * p.m[0][1] + m[2][3] * p.m[0][2] + m[3][3] * p.m[0][3],
                m[0][3] * p.m[1][0] + m[1][3] * p.m[1][1] + m[2][3] * p.m[1][2] + m[3][3] * p.m[1][3],
                m[0][3] * p.m[2][0] + m[1][3] * p.m[2][1] + m[2][3] * p.m[2][2] + m[3][3] * p.m[2][3],
                m[0][3] * p.m[3][0] + m[1][3] * p.m[3][1] + m[2][3] * p.m[3][2] + m[3][3] * p.m[3][3]);
        }

        template<class T>
        _vec4<T> _mat4::operator* (const _vec4<T>& vec) const
        {
            return _vec4<T>(
                T(m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0] * vec.w),
                T(m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1] * vec.w),
                T(m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2] * vec.w),
                T(m[0][3] * vec.x + m[1][3] * vec.y + m[2][3] * vec.z + m[3][3] * vec.w));
        }

        _mat4 _mat4::operator* (const matReal n) const
        {
            return _mat4(
                m[0][0] * n, m[0][1] * n, m[0][2] * n, m[0][3] * n,
                m[1][0] * n, m[1][1] * n, m[1][2] * n, m[1][3] * n,
                m[2][0] * n, m[2][1] * n, m[2][2] * n, m[2][3] * n,
                m[3][0] * n, m[3][1] * n, m[3][2] * n, m[3][3] * n);
        }

        _mat4 _mat4::operator+ (const mat4& p) const
        {
            return _mat4(
                m[0][0] + p.m[0][0], m[0][1] + p.m[0][1],
                m[0][2] + p.m[0][2], m[0][3] + p.m[0][3],
                m[1][0] + p.m[1][0], m[1][1] + p.m[1][1],
                m[1][2] + p.m[1][2], m[1][3] + p.m[1][3],
                m[2][0] + p.m[2][0], m[2][1] + p.m[2][1],
                m[2][2] + p.m[2][2], m[2][3] + p.m[2][3],
                m[3][0] + p.m[3][0], m[3][1] + p.m[3][1],
                m[3][2] + p.m[3][2], m[3][3] + p.m[3][3]);
        }

        _mat4 _mat4::operator- (const mat4& p) const
        {
            return _mat4(
                m[0][0] - p.m[0][0], m[0][1] - p.m[0][1],
                m[0][2] - p.m[0][2], m[0][3] - p.m[0][3],
                m[1][0] - p.m[1][0], m[1][1] - p.m[1][1],
                m[1][2] - p.m[1][2], m[1][3] - p.m[1][3],
                m[2][0] - p.m[2][0], m[2][1] - p.m[2][1],
                m[2][2] - p.m[2][2], m[2][3] - p.m[2][3],
                m[3][0] - p.m[3][0], m[3][1] - p.m[3][1],
                m[3][2] - p.m[3][2], m[3][3] - p.m[3][3]);
        }

        _mat4 _mat4::operator / (const matReal n) const
        {
            return _mat4(
                m[0][0] / n, m[0][1] / n, m[0][2] / n, m[0][3] / n,
                m[1][0] / n, m[1][1] / n, m[1][2] / n, m[1][3] / n,
                m[2][0] / n, m[2][1] / n, m[2][2] / n, m[2][3] / n,
                m[3][0] / n, m[3][1] / n, m[3][2] / n, m[3][3] / n);
        }

        _mat4 _mat4::operator+ (const matReal n) const
        {
            return _mat4(
                m[0][0] + n, m[0][1] + n, m[0][2] + n, m[0][3] + n,
                m[1][0] + n, m[1][1] + n, m[1][2] + n, m[1][3] + n,
                m[2][0] + n, m[2][1] + n, m[2][2] + n, m[2][3] + n,
                m[3][0] + n, m[3][1] + n, m[3][2] + n, m[3][3] + n);
        }

        _mat4 _mat4::operator- (const matReal n) const
        {
            return _mat4(
                m[0][0] - n, m[0][1] - n, m[0][2] - n, m[0][3] - n,
                m[1][0] - n, m[1][1] - n, m[1][2] - n, m[1][3] - n,
                m[2][0] - n, m[2][1] - n, m[2][2] - n, m[2][3] - n,
                m[3][0] - n, m[3][1] - n, m[3][2] - n, m[3][3] - n);
        }

        void _mat4::operator *= (const _mat4& p)
        {
            matReal t1, t2, t3, t4;
            t1 = m[0][0] * p.m[0][0] + m[1][0] * p.m[0][1] + m[2][0] * p.m[0][2] + m[3][0] * p.m[0][3];
            t2 = m[0][0] * p.m[1][0] + m[1][0] * p.m[1][1] + m[2][0] * p.m[1][2] + m[3][0] * p.m[1][3];
            t3 = m[0][0] * p.m[2][0] + m[1][0] * p.m[2][1] + m[2][0] * p.m[2][2] + m[3][0] * p.m[2][3];
            t4 = m[0][0] * p.m[3][0] + m[1][0] * p.m[3][1] + m[2][0] * p.m[3][2] + m[3][0] * p.m[3][3];
            m[0][0] = t1; m[1][0] = t2; m[2][0] = t3; m[3][0] = t4;
            t1 = m[0][1] * p.m[0][0] + m[1][1] * p.m[0][1] + m[2][1] * p.m[0][2] + m[3][1] * p.m[0][3];
            t2 = m[0][1] * p.m[1][0] + m[1][1] * p.m[1][1] + m[2][1] * p.m[1][2] + m[3][1] * p.m[1][3];
            t3 = m[0][1] * p.m[2][0] + m[1][1] * p.m[2][1] + m[2][1] * p.m[2][2] + m[3][1] * p.m[2][3];
            t4 = m[0][1] * p.m[3][0] + m[1][1] * p.m[3][1] + m[2][1] * p.m[3][2] + m[3][1] * p.m[3][3];
            m[0][1] = t1; m[1][1] = t2; m[2][1] = t3; m[3][1] = t4;
            t1 = m[0][2] * p.m[0][0] + m[1][2] * p.m[0][1] + m[2][2] * p.m[0][2] + m[3][2] * p.m[0][3];
            t2 = m[0][2] * p.m[1][0] + m[1][2] * p.m[1][1] + m[2][2] * p.m[1][2] + m[3][2] * p.m[1][3];
            t3 = m[0][2] * p.m[2][0] + m[1][2] * p.m[2][1] + m[2][2] * p.m[2][2] + m[3][2] * p.m[2][3];
            t4 = m[0][2] * p.m[3][0] + m[1][2] * p.m[3][1] + m[2][2] * p.m[3][2] + m[3][2] * p.m[3][3];
            m[0][2] = t1; m[1][2] = t2; m[2][2] = t3; m[3][2] = t4;
            t1 = m[0][3] * p.m[0][0] + m[1][3] * p.m[0][1] + m[2][3] * p.m[0][2] + m[3][3] * p.m[0][3];
            t2 = m[0][3] * p.m[1][0] + m[1][3] * p.m[1][1] + m[2][3] * p.m[1][2] + m[3][3] * p.m[1][3];
            t3 = m[0][3] * p.m[2][0] + m[1][3] * p.m[2][1] + m[2][3] * p.m[2][2] + m[3][3] * p.m[2][3];
            t4 = m[0][3] * p.m[3][0] + m[1][3] * p.m[3][1] + m[2][3] * p.m[3][2] + m[3][3] * p.m[3][3];
            m[0][3] = t1; m[1][3] = t2; m[2][3] = t3; m[3][3] = t4;
        }

        void _mat4::operator *= (const matReal n)
        {
            m[0][0] *= n; m[0][1] *= n; m[0][2] *= n; m[0][3] *= n;
            m[1][0] *= n; m[1][1] *= n; m[1][2] *= n; m[1][3] *= n;
            m[2][0] *= n; m[2][1] *= n; m[2][2] *= n; m[2][3] *= n;
            m[3][0] *= n; m[3][1] *= n; m[3][2] *= n; m[3][3] *= n;
        }

        void _mat4::operator /= (const matReal n)
        {
            m[0][0] /= n; m[0][1] /= n; m[0][2] /= n; m[0][3] /= n;
            m[1][0] /= n; m[1][1] /= n; m[1][2] /= n; m[1][3] /= n;
            m[2][0] /= n; m[2][1] /= n; m[2][2] /= n; m[2][3] /= n;
            m[3][0] /= n; m[3][1] /= n; m[3][2] /= n; m[3][3] /= n;
        }

        void _mat4::operator += (const _mat4& p)
        {
            m[0][0] += p.m[0][0]; m[0][1] += p.m[0][1];
            m[0][2] += p.m[0][2]; m[0][3] += p.m[0][3];
            m[1][0] += p.m[1][0]; m[1][1] += p.m[1][1];
            m[1][2] += p.m[1][2]; m[1][3] += p.m[1][3];
            m[2][0] += p.m[2][0]; m[2][1] += p.m[2][1];
            m[2][2] += p.m[2][2]; m[2][3] += p.m[2][3];
            m[3][0] += p.m[3][0]; m[3][1] += p.m[3][1];
            m[3][2] += p.m[3][2]; m[3][3] += p.m[3][3];
        }

        void _mat4::operator += (const matReal n)
        {
            m[0][0] += n; m[0][1] += n; m[0][2] += n; m[0][3] += n;
            m[1][0] += n; m[1][1] += n; m[1][2] += n; m[1][3] += n;
            m[2][0] += n; m[2][1] += n; m[2][2] += n; m[2][3] += n;
            m[3][0] += n; m[3][1] += n; m[3][2] += n; m[3][3] += n;
        }

        void _mat4::operator  -= (const _mat4& p)
        {
            m[0][0] -= p.m[0][0]; m[0][1] -= p.m[0][1];
            m[0][2] -= p.m[0][2]; m[0][3] -= p.m[0][3];
            m[1][0] -= p.m[1][0]; m[1][1] -= p.m[1][1];
            m[1][2] -= p.m[1][2]; m[1][3] -= p.m[1][3];
            m[2][0] -= p.m[2][0]; m[2][1] -= p.m[2][1];
            m[2][2] -= p.m[2][2]; m[2][3] -= p.m[2][3];
            m[3][0] -= p.m[3][0]; m[3][1] -= p.m[3][1];
            m[3][2] -= p.m[3][2]; m[3][3] -= p.m[3][3];
        }

        void _mat4::operator  -= (const matReal n)
        {
            m[0][0] -= n; m[0][1] -= n; m[0][2] -= n; m[0][3] -= n;
            m[1][0] -= n; m[1][1] -= n; m[1][2] -= n; m[1][3] -= n;
            m[2][0] -= n; m[2][1] -= n; m[2][2] -= n; m[2][3] -= n;
            m[3][0] -= n; m[3][1] -= n; m[3][2] -= n; m[3][3] -= n;
        }

        _mat4& _mat4::initRotation(const matReal angle, const matReal x,
            const matReal y, const matReal z)
        {
            matReal c = cos(angle);
            matReal s = sin(angle);
            matReal iC = matScalarOne - c;

            m[0][0] = x*x + (matScalarOne - x*x)*c;
            m[0][1] = iC*x*y + z*s;
            m[0][2] = iC*x*z - y*s;
            m[0][3] = matScalarNull;
            m[1][0] = iC*x*y - z*s;
            m[1][1] = y*y + (matScalarOne - y*y)*c;
            m[1][2] = iC*y*z + x*s;
            m[1][3] = matScalarNull;
            m[2][0] = iC*x*z + y*s;
            m[2][1] = iC*y*z - x*s;
            m[2][2] = z*z + (matScalarOne - z*z)*c;
            m[2][3] = matScalarNull;
            m[3][0] = matScalarNull;
            m[3][1] = matScalarNull;
            m[3][2] = matScalarNull;
            m[3][3] = matScalarOne;

            return *this;
        }

        bool _mat4::operator == (const _mat4& p) const
        {
            return
                m[0][0] == p.m[0][0] && m[0][1] == p.m[0][1] &&
                m[0][2] == p.m[0][2] && m[0][3] == p.m[0][3] &&
                m[1][0] == p.m[1][0] && m[1][1] == p.m[1][1] &&
                m[1][2] == p.m[1][2] && m[1][3] == p.m[1][3] &&
                m[2][0] == p.m[2][0] && m[2][1] == p.m[2][1] &&
                m[2][2] == p.m[2][2] && m[2][3] == p.m[2][3] &&
                m[3][0] == p.m[3][0] && m[3][1] == p.m[3][1] &&
                m[3][2] == p.m[3][2] && m[3][3] == p.m[3][3];
        }

        bool _mat4::operator != (const _mat4& p) const
        {
            return
                m[0][0] != p.m[0][0] || m[0][1] != p.m[0][1] ||
                m[0][2] != p.m[0][2] || m[0][3] != p.m[0][3] ||
                m[1][0] != p.m[1][0] || m[1][1] != p.m[1][1] ||
                m[1][2] != p.m[1][2] || m[1][3] != p.m[1][3] ||
                m[2][0] != p.m[2][0] || m[2][1] != p.m[2][1] ||
                m[2][2] != p.m[2][2] || m[2][3] != p.m[2][3] ||
                m[3][0] != p.m[3][0] || m[3][1] != p.m[3][1] ||
                m[3][2] != p.m[3][2] || m[3][3] != p.m[3][3];
        }

        _mat4& _mat4::initTranslation(const matReal x, const matReal y, const matReal z)
        {
            reset();
            m[3][0] = x;
            m[3][1] = y;
            m[3][2] = z;
            return *this;
        }

        _mat4& _mat4::initScale(const matReal x, const matReal y, const matReal z)
        {
            reset();
            m[0][0] = x;
            m[1][1] = y;
            m[2][2] = z;
            return *this;
        }

        inline _mat4& mat4::initRotation(matReal angle, const vec3& axis)
        {
            return initRotation(angle, axis.x, axis.y, axis.z);
        }

        inline _mat4& _mat4::initTranslation(vec3& trans)
        {
            return initTranslation(trans.x, trans.y, trans.z);
        }

        inline _mat4& _mat4::initScale(vec3& scale)
        {
            return initScale(scale.x, scale.y, scale.z);
        }

        _mat4& _mat4::initProjection(const matReal fov, const matReal width,
            const matReal height, const matReal znear, const matReal zfar)
        {
            reset();

            matReal tanHalfFOV = tan(fov / matScalarTwo);
            m[1][1] = matScalarOne / tanHalfFOV;
            m[0][0] = -m[1][1] / (width / height); //negative?
            m[2][2] = zfar*(matScalarOne / (zfar - znear));
            m[3][2] = (-zfar*znear)*(matScalarOne / (zfar - znear));
            m[2][3] = matScalarOne;
            m[3][3] = matScalarNull;
            return *this;
        }

        _mat4& _mat4::initOrthoProjection(const matReal left_, const matReal right_,
            const matReal bottom_, const matReal top_, const matReal near_, const matReal far_)
        {
            reset();
            m[0][0] = matScalarTwo / (right_ - left_);
            m[1][1] = matScalarTwo / (top_ - bottom_);
            m[2][2] = -matScalarTwo / (far_ - near_);
            m[3][0] = -((right_ + left_) / (right_ - left_));
            m[3][1] = -((top_ + bottom_) / (top_ - bottom_));
            m[3][2] = (far_ + near_) / (far_ - near_);
            return *this;
        }

        _mat4 _mat4::getTranspose()
        {
            return _mat4(m[0][0], m[0][1], m[0][2], m[0][3],
                         m[1][0], m[1][1], m[1][2], m[1][3],
                         m[2][0], m[2][1], m[2][2], m[2][3],
                         m[3][0], m[3][1], m[3][2], m[3][3]);
        }

        _mat3::_mat3()
        {
            reset();
        }

        _mat3::_mat3(const matReal n)
        {
            reset();
            m[0][0] = n;
            m[1][1] = n;
            m[2][2] = n;
        }

        _mat3::_mat3(const matReal m00, const matReal m10, const matReal m20,
            const matReal m01, const matReal m11, const matReal m21,
            const matReal m02, const matReal m12, const matReal m22)
        {
            m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
            m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
            m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
        }

        void _mat3::reset()
        {
            m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
            m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
            m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
        }

        _mat3 _mat3::operator * (const _mat3& p) const
        {
            return _mat3(
                m[0][0] * p.m[0][0] + m[1][0] * p.m[0][1] + m[2][0] * p.m[0][2],
                m[0][0] * p.m[1][0] + m[1][0] * p.m[1][1] + m[2][0] * p.m[1][2],
                m[0][0] * p.m[2][0] + m[1][0] * p.m[2][1] + m[2][0] * p.m[2][2],

                m[0][1] * p.m[0][0] + m[1][1] * p.m[0][1] + m[2][1] * p.m[0][2],
                m[0][1] * p.m[1][0] + m[1][1] * p.m[1][1] + m[2][1] * p.m[1][2],
                m[0][1] * p.m[2][0] + m[1][1] * p.m[2][1] + m[2][1] * p.m[2][2],

                m[0][2] * p.m[0][0] + m[1][2] * p.m[0][1] + m[2][2] * p.m[0][2],
                m[0][2] * p.m[1][0] + m[1][2] * p.m[1][1] + m[2][2] * p.m[1][2],
                m[0][2] * p.m[2][0] + m[1][2] * p.m[2][1] + m[2][2] * p.m[2][2]);
        }

        vec3 _mat3::operator* (const vec3& vec) const
        {
            return vec3(
                m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z,
                m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z,
                m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z);
        }

        _mat3 _mat3::operator* (const matReal n) const
        {
            return _mat3(
                m[0][0] * n, m[0][1] * n, m[0][2] * n,
                m[1][0] * n, m[1][1] * n, m[1][2] * n,
                m[2][0] * n, m[2][1] * n, m[2][2] * n);
        }

        _mat3 _mat3::operator+ (const _mat3& p) const
        {
            return _mat3(
                m[0][0] + p.m[0][0], m[0][1] + p.m[0][1], m[0][2] + p.m[0][2],
                m[1][1] + p.m[1][1], m[1][0] + p.m[1][0], m[1][2] + p.m[1][2],
                m[2][0] + p.m[2][0], m[2][1] + p.m[2][1], m[2][2] + p.m[2][2]);
        }

        _mat3 _mat3::operator- (const _mat3& p) const
        {
            return _mat3(
                m[0][0] - p.m[0][0], m[0][1] - p.m[0][1], m[0][2] - p.m[0][2],
                m[1][1] - p.m[1][1], m[1][0] - p.m[1][0], m[1][2] - p.m[1][2],
                m[2][0] - p.m[2][0], m[2][1] - p.m[2][1], m[2][2] - p.m[2][2]);
        }

        _mat3 _mat3::operator / (const matReal n) const
        {
            return _mat3(
                m[0][0] / n, m[0][1] / n, m[0][2] / n,
                m[1][0] / n, m[1][1] / n, m[1][2] / n,
                m[2][0] / n, m[2][1] / n, m[2][2] / n);
        }

        _mat3 _mat3::operator+ (const matReal n) const
        {
            return _mat3(
                m[0][0] + n, m[0][1] + n, m[0][2] + n,
                m[1][0] + n, m[1][1] + n, m[1][2] + n,
                m[2][0] + n, m[2][1] + n, m[2][2] + n);
        }

        _mat3 _mat3::operator- (const matReal n) const
        {
            return _mat3(
                m[0][0] - n, m[0][1] - n, m[0][2] - n,
                m[1][0] - n, m[1][1] - n, m[1][2] - n,
                m[2][0] - n, m[2][1] - n, m[2][2] - n);
        }

        void _mat3::operator *= (const _mat3& p)
        {
            matReal t1, t2, t3;
            t1 = m[0][0] * p.m[0][0] + m[1][0] * p.m[0][1] + m[2][0] * p.m[0][2];
            t2 = m[0][0] * p.m[1][0] + m[1][0] * p.m[1][1] + m[2][0] * p.m[1][2];
            t3 = m[0][0] * p.m[2][0] + m[1][0] * p.m[2][1] + m[2][0] * p.m[2][2];
            m[0][0] = t1; m[1][0] = t2; m[2][0] = t3;
            t1 = m[0][1] * p.m[0][0] + m[1][1] * p.m[0][1] + m[2][1] * p.m[0][2];
            t2 = m[0][1] * p.m[1][0] + m[1][1] * p.m[1][1] + m[2][1] * p.m[1][2];
            t3 = m[0][1] * p.m[2][0] + m[1][1] * p.m[2][1] + m[2][1] * p.m[2][2];
            m[0][1] = t1; m[1][1] = t2; m[2][1] = t3;
            t1 = m[0][2] * p.m[0][0] + m[1][2] * p.m[0][1] + m[2][2] * p.m[0][2];
            t2 = m[0][2] * p.m[1][0] + m[1][2] * p.m[1][1] + m[2][2] * p.m[1][2];
            t3 = m[0][2] * p.m[2][0] + m[1][2] * p.m[2][1] + m[2][2] * p.m[2][2];
            m[0][2] = t1; m[1][2] = t2; m[2][2] = t3;
        }

        void _mat3::operator *= (const matReal n)
        {
            m[0][0] *= n; m[0][1] *= n; m[0][2] *= n;
            m[1][0] *= n; m[1][1] *= n; m[1][2] *= n;
            m[2][0] *= n; m[2][1] *= n; m[2][2] *= n;
        }

        void _mat3::operator /= (const matReal n)
        {
            m[0][0] /= n; m[0][1] /= n; m[0][2] /= n;
            m[1][0] /= n; m[1][1] /= n; m[1][2] /= n;
            m[2][0] /= n; m[2][1] /= n; m[2][2] /= n;
        }

        void _mat3::operator += (const _mat3& p)
        {
            m[0][0] += p.m[0][0]; m[0][1] += p.m[0][1]; m[0][2] += p.m[0][2];
            m[1][0] += p.m[1][0]; m[1][1] += p.m[1][1]; m[1][2] += p.m[1][2];
            m[2][0] += p.m[2][0]; m[2][1] += p.m[2][1]; m[2][2] += p.m[2][2];
        }

        void _mat3::operator += (const matReal n)
        {
            m[0][0] += n; m[0][1] += n; m[0][2] += n;
            m[1][0] += n; m[1][1] += n; m[1][2] += n;
            m[2][0] += n; m[2][1] += n; m[2][2] += n;
        }

        void _mat3::operator  -= (const _mat3& p)
        {
            m[0][0] -= p.m[0][0]; m[0][1] -= p.m[0][1]; m[0][2] -= p.m[0][2];
            m[1][0] -= p.m[1][0]; m[1][1] -= p.m[1][1]; m[1][2] -= p.m[1][2];
            m[2][0] -= p.m[2][0]; m[2][1] -= p.m[2][1]; m[2][2] -= p.m[2][2];
        }

        void _mat3::operator  -= (const matReal n)
        {
            m[0][0] -= n; m[0][1] -= n; m[0][2] -= n;
            m[1][0] -= n; m[1][1] -= n; m[1][2] -= n;
            m[2][0] -= n; m[2][1] -= n; m[2][2] -= n;
        }

        bool _mat3::operator == (const _mat3& p) const
        {
            return
                m[0][0] == p.m[0][0] && m[0][1] == p.m[0][1] && m[0][2] == p.m[0][2] &&
                m[1][0] == p.m[1][0] && m[1][1] == p.m[1][1] && m[1][2] == p.m[1][2] &&
                m[2][0] == p.m[2][0] && m[2][1] == p.m[2][1] && m[2][2] == p.m[2][2];
        }

        bool _mat3::operator != (const _mat3& p) const
        {
            return
                m[0][0] != p.m[0][0] || m[0][1] != p.m[0][1] || m[0][2] != p.m[0][2] ||
                m[1][0] != p.m[1][0] || m[1][1] != p.m[1][1] || m[1][2] != p.m[1][2] ||
                m[2][0] != p.m[2][0] || m[2][1] != p.m[2][1] || m[2][2] != p.m[2][2];
        }

        _mat3 _mat3::getTranspose() const
        {
            return _mat3(m[0][0], m[0][1], m[0][2],
                         m[1][0], m[1][1], m[1][2],
                         m[2][0], m[2][1], m[2][2]);
        }

        matReal _mat3::det() const
        {
            return (m[0][0]*m[1][1]*m[2][2] + m[1][0]*m[2][1]*m[0][2] + m[2][0]*m[0][1]*m[1][2])-
                    (m[2][0]*m[1][1]*m[0][2] + m[0][0]*m[2][1]*m[1][2] + m[1][0]*m[0][1]*m[2][2]);
        }

        _mat3 _mat3::getInverse() const
        {
            matReal determinent = det();
            if(determinent==0)
                return _mat3();

            matReal a = m[0][0];
            matReal b = m[1][0];
            matReal c = m[2][0];
            matReal d = m[0][1];
            matReal e = m[1][1];
            matReal f = m[2][1];
            matReal g = m[0][2];
            matReal h = m[1][2];
            matReal i = m[2][2];

            return _mat3(e*i-f*h,f*g-d*i,d*h-e*g,
                         c*h-b*i,a*i-c*g,b*g-a*h,
                         b*f-c*e,c*d-a*f,a*e-b*d)*(1/determinent);
        }

        _mat2::_mat2()
        {
            reset();
        }

        _mat2::_mat2(const matReal n)
        {
            reset();
            m[0][0] = n; m[1][1] = n;
        }

        _mat2::_mat2(const matReal m00, const matReal m10,
            const matReal m01, const matReal m11)
        {
            m[0][0] = m00; m[0][1] = m01;
            m[1][0] = m10; m[1][1] = m11;
        }

        void _mat2::reset()
        {
            m[0][0] = 1; m[0][1] = 0;
            m[1][0] = 0; m[1][1] = 1;
        }

        _mat2 _mat2::operator * (const _mat2& p) const
        {
            return _mat2(
                m[0][0] * p.m[0][0] + m[1][0] * p.m[0][1],
                m[0][0] * p.m[1][0] + m[1][0] * p.m[1][1],
                m[0][1] * p.m[0][0] + m[1][1] * p.m[0][1],
                m[0][1] * p.m[1][0] + m[1][1] * p.m[1][1]);
        }

        vec2 _mat2::operator* (const vec2& vec) const
        {
            return vec2(
                m[0][0] * vec.x + m[1][0] * vec.y,
                m[0][1] * vec.x + m[1][1] * vec.y);
        }

        _mat2 _mat2::operator* (const matReal n) const
        {
            return _mat2(
                m[0][0] * n, m[0][1] * n,
                m[1][0] * n, m[1][1] * n);
        }

        _mat2 _mat2::operator+ (const _mat2& p) const
        {
            return _mat2(
                m[0][0] + p.m[0][0], m[0][1] + p.m[0][1],
                m[1][0] + p.m[1][0], m[1][1] + p.m[1][1]);
        }

        _mat2 _mat2::operator- (const _mat2& p) const
        {
            return _mat2(
                m[0][0] - p.m[0][0], m[0][1] - p.m[0][1],
                m[1][0] - p.m[1][0], m[1][1] - p.m[1][1]);
        }

        _mat2 _mat2::operator / (const matReal n) const
        {
            return _mat2(
                m[0][0] / n, m[0][1] / n,
                m[1][0] / n, m[1][1] / n);
        }

        _mat2 _mat2::operator+ (const matReal n) const
        {
            return _mat2(
                m[0][0] + n, m[0][1] + n,
                m[1][0] + n, m[1][1] + n);
        }

        _mat2 _mat2::operator- (const matReal n) const
        {
            return _mat2(
                m[0][0] - n, m[0][1] - n,
                m[1][0] - n, m[1][1] - n);
        }

        void _mat2::operator *= (const _mat2& p)
        {
            matReal t1, t2;
            t1 = m[0][0] * p.m[0][0] + m[1][0] * p.m[0][1];
            t2 = m[0][0] * p.m[1][0] + m[1][0] * p.m[1][1];
            m[0][0] = t1; m[1][0] = t2;
            t1 = m[0][1] * p.m[0][0] + m[1][1] * p.m[0][1];
            t2 = m[0][1] * p.m[1][0] + m[1][1] * p.m[1][1];
            m[0][1] = t1; m[1][1] = t2;
        }

        void _mat2::operator *= (const matReal n)
        {
            m[0][0] *= n; m[0][1] *= n;
            m[1][0] *= n; m[1][1] *= n;
        }

        void _mat2::operator /= (const matReal n)
        {
            m[0][0] /= n; m[0][1] /= n;
            m[1][0] /= n; m[1][1] /= n;
        }

        void _mat2::operator += (const _mat2& p)
        {
            m[0][0] += p.m[0][0]; m[0][1] += p.m[0][1];
            m[1][0] += p.m[1][0]; m[1][1] += p.m[1][1];
        }

        void _mat2::operator += (const matReal n)
        {
            m[0][0] += n; m[0][1] += n;
            m[1][0] += n; m[1][1] += n;
        }

        void _mat2::operator  -= (const _mat2& p)
        {
            m[0][0] -= p.m[0][0]; m[0][1] -= p.m[0][1];
            m[1][0] -= p.m[1][0]; m[1][1] -= p.m[1][1];
        }

        void _mat2::operator  -= (const matReal n)
        {
            m[0][0] -= n; m[0][1] -= n;
            m[1][0] -= n; m[1][1] -= n;
        }

        bool _mat2::operator == (const _mat2& p) const
        {
            return
                m[0][0] == p.m[0][0] && m[0][1] == p.m[0][1] &&
                m[1][0] == p.m[1][0] && m[1][1] == p.m[1][1];
        }

        bool _mat2::operator != (const _mat2& p) const
        {
            return
                m[0][0] != p.m[0][0] || m[0][1] != p.m[0][1] ||
                m[1][0] != p.m[1][0] || m[1][1] != p.m[1][1];
        }

        _mat2 _mat2::getTranspose()
        {
            return _mat2(m[0][0], m[0][1],
                         m[1][0], m[1][1]);
        }

    }

//Force vector instantiatios for types:

#define FORCE_VEC_INST(type) \
    template class tmp::_vec2<type>; \
    template class tmp::_vec3<type>; \
    template class tmp::_vec4<type>; \
    template type dot<type>(const tmp::_vec2<type>&, const tmp::_vec2<type>&); \
    template type dot<type>(const tmp::_vec3<type>&, const tmp::_vec3<type>&); \
    template type dot<type>(const tmp::_vec4<type>&, const tmp::_vec4<type>&); \
    template tmp::_vec3<type> cross<type>(const tmp::_vec3<type>&, const tmp::_vec3<type>&); \

    FORCE_VEC_INST(float)
    FORCE_VEC_INST(int)
    FORCE_VEC_INST(double)
    FORCE_VEC_INST(long long)
    FORCE_VEC_INST(short)


#undef FORCE_VEC_INST

}
