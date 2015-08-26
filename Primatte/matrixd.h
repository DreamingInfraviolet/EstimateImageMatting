#pragma once

/*
 * A series of vec3{2,3,4} and mat{2,3,4} classes.
 * @author Anima.
 */

namespace math
{
//Affects vecX and matX

#ifdef MATRIX_D_DOUBLE
    typedef double matReal;
#else
    typedef float matReal;
#endif

#ifdef MATRIX_D_LONG
    typedef long long matInt;
#else
    typedef int matInt;
#endif

    namespace tmp
    {
        template <class T>
        class _vec2
        {
        public:
            T x, y;

            _vec2<T>();
            _vec2<T>(const T x_, const T y_);
            explicit _vec2<T>(const T n);
            matReal angle() const;
            T lengthSquared() const;
            matReal length() const;
            matReal distance(const _vec2<T>& v) const;
            matReal proj(const _vec2<T>& v) const;
            T distanceSquared(const _vec2<T>& v) const;
            _vec2<T> operator * (const T n) const;
            _vec2<T> operator / (const T n) const;
            _vec2<T> operator + (const T n) const;
            _vec2<T> operator - (const T n) const;
            _vec2<T> operator + (const _vec2<T>& vec) const;
            _vec2<T> operator - (const _vec2<T>& vec) const;
            _vec2<T> operator * (const _vec2<T>& vec) const;
            _vec2<T> operator / (const _vec2<T>& vec) const;
            void operator *= (const T f);
            void operator /= (const T f);
            void operator += (const T f);
            void operator -= (const T f);
            void operator += (const _vec2<T>& vec);
            void operator -= (const _vec2<T>& vec);
            void operator *= (const _vec2<T>& vec);
            void operator /= (const _vec2<T>& vec);
            bool operator == (const _vec2<T>& vec) const;
            bool operator == (T f) const;
            bool operator != (const _vec2<T>& vec) const;
            bool operator != (T f) const;
            bool operator < (const _vec2<T>& vec) const;
            bool operator <= (const _vec2<T>& vec) const;
            bool operator > (const _vec2<T>& vec) const;
            bool operator >= (const _vec2<T>& vec) const;

            _vec2<T>& normalize();
            void reset();

            _vec2<T> operator % (const T n) const;
            void operator %= (const T n);
            _vec2<T> operator % (const _vec2<T>& vec) const;
            void operator %= (const _vec2<T>& vec);


            _vec2<T> xy() const; _vec2<T> yx() const;
        };

        template<class T>
        class _vec3
        {
        public:
            T x, y, z;

            _vec3();
            _vec3(const T x_, const T y_, const T z_);
            _vec3(const _vec2<T>& v, const T f);
            _vec3(const T f, const _vec2<T>& v);
            explicit _vec3(const T f);
            T lengthSquared() const;
            matReal length() const;
            matReal distance(const _vec3<T>& v) const;
            T distanceSquared(const _vec3<T>& v) const;
            matReal proj(const _vec3<T>& v) const;
            _vec3<T> operator * (const T n) const;
            _vec3<T> operator / (const T n) const;
            _vec3<T> operator + (const T n) const;
            _vec3<T> operator - (const T n) const;
            _vec3<T> operator + (const _vec3<T>& vec) const;
            _vec3<T> operator - (const _vec3<T>& vec) const;
            _vec3<T> operator * (const _vec3<T>& vec) const;
            _vec3<T> operator / (const _vec3<T>& vec) const;
            void operator *= (const T n);
            void operator /= (const T n);
            void operator += (const T n);
            void operator -= (const T n);
            void operator += (const _vec3<T>& vec);
            void operator -= (const _vec3<T>& vec);
            void operator *= (const _vec3<T>& vec);
            void operator /= (const _vec3<T>& vec);
            bool operator == (const _vec3<T>& vec) const;
            bool operator == (T f) const;
            bool operator != (const _vec3<T>& vec) const;
            bool operator != (T f) const;
            bool operator < (const _vec3<T>& vec) const;
            bool operator <= (const _vec3<T>& vec) const;
            bool operator > (const _vec3<T>& vec) const;
            bool operator >= (const _vec3<T>& vec) const;
            _vec3<T>& normalize();

            _vec3<T>& rotate(const matReal angle, const _vec3<matReal>& axis);

            _vec3<T> operator % (const T n) const;
            void operator %= (const T n);
            _vec3<T> operator % (const _vec3<T>& vec) const;
            void operator %= (const _vec3<T>& vec);


            void reset();

            _vec3<T> xyz() const; _vec3<T> xzy() const; _vec3<T> yxz() const;
            _vec3<T> yzx() const; _vec3<T> zxy() const; _vec3<T> zyx() const;
            _vec2<T> xy() const; _vec2<T> yx() const; _vec2<T> xz() const;
            _vec2<T> zx() const; _vec2<T> yz() const; _vec2<T> zy() const;
        };

        template<class T>
        class _vec4
        {
        public:
            T x, y, z, w;

            _vec4();
            _vec4(const T x_, const T y_, const T z_, const T w_);
            _vec4(const _vec2<T>& v1, const _vec2<T>& v2);
            _vec4(const T f1, const T f2, const _vec2<T>& v);
            _vec4(const _vec2<T>& v, const T f1, const T f2);
            _vec4(const T f1, const _vec2<T>& v, const T f2);
            _vec4(const _vec3<T>& v, const T f);
            _vec4(const T f, const _vec3<T>& v);
            explicit _vec4(const T f);

            matReal length() const;
            T lengthSquared() const;
            matReal distance(const _vec4<T>& v) const;
            T distanceSquared(const _vec4<T>& v) const;
            matReal proj(const _vec4<T>& v) const;
            _vec4<T> operator * (const T n) const;
            _vec4<T> operator / (const T n) const;
            _vec4<T> operator + (const T n) const;
            _vec4<T> operator - (const T n) const;
            _vec4<T> operator + (const _vec4<T>& vec) const;
            _vec4<T> operator - (const _vec4<T>& vec) const;
            _vec4<T> operator * (const _vec4<T>& vec) const;
            _vec4<T> operator / (const _vec4<T>& vec) const;
            void operator *= (const T n);
            void operator /= (const T n);
            void operator += (const T n);
            void operator -= (const T n);
            void operator += (const _vec4<T>& vec);
            void operator -= (const _vec4<T>& vec);
            void operator *= (const _vec4<T>& vec);
            void operator /= (const _vec4<T>& vec);
            bool operator == (const _vec4<T>& vec) const;
            bool operator == (T f) const;
            bool operator != (const _vec4<T>& vec) const;
            bool operator != (T f) const;
            bool operator < (const _vec4<T>& vec) const;
            bool operator <= (const _vec4<T>& vec) const;
            bool operator > (const _vec4<T>& vec) const;
            bool operator >= (const _vec4<T>& vec) const;

            _vec4<T> operator % (const T n) const;
            void operator %= (const T n);
            _vec4<T> operator % (const _vec4<T>& vec) const;
            void operator %= (const _vec4<T>& vec);

            _vec4<T>& normalize();

            void reset();

            _vec4<T> xyzw() const; _vec4<T> xywz() const; _vec4<T> xzwy() const;
            _vec4<T> xzyw() const; _vec4<T> xwzy() const; _vec4<T> xwyz() const;
            _vec4<T> yxzw() const; _vec4<T> yxwz() const; _vec4<T> yzwx() const;
            _vec4<T> yzxw() const; _vec4<T> ywzx() const; _vec4<T> ywxz() const;
            _vec4<T> zyxw() const; _vec4<T> zywx() const; _vec4<T> zxwy() const;
            _vec4<T> zxyw() const; _vec4<T> zwxy() const; _vec4<T> zwyx() const;
            _vec4<T> wyzx() const; _vec4<T> wyxz() const; _vec4<T> wzxy() const;
            _vec4<T> wzyx() const; _vec4<T> wxzy() const; _vec4<T> wxyz() const;
            _vec3<T> xyz() const; _vec3<T> xzy() const; _vec3<T> yxz() const;
            _vec3<T> yzx() const; _vec3<T> zxy() const; _vec3<T> zyx() const;
            _vec3<T> wyz() const; _vec3<T> wzy() const; _vec3<T> ywz() const;
            _vec3<T> yzw() const; _vec3<T> zwy() const; _vec3<T> zyw() const;
            _vec3<T> xyw() const; _vec3<T> xwy() const; _vec3<T> yxw() const;
            _vec3<T> ywx() const; _vec3<T> wxy() const; _vec3<T> wyx() const;
            _vec3<T> xwz() const; _vec3<T> xzw() const; _vec3<T> wxz() const;
            _vec3<T> wzx() const; _vec3<T> zxw() const; _vec3<T> zwx() const;
            _vec2<T> xy() const; _vec2<T> yx() const; _vec2<T> xz() const;
            _vec2<T> zx() const; _vec2<T> xw() const; _vec2<T> wx() const;
            _vec2<T> yz() const; _vec2<T> zy() const; _vec2<T> wz() const;
            _vec2<T> zw() const;
        };

        class _mat4
        {
        public:
            matReal m[4][4];

            _mat4();
            explicit _mat4(const matReal n);
            _mat4(const matReal m00, const matReal m01, const matReal m02, const matReal m03,
                const matReal m10, const matReal m11, const matReal m12, const matReal m13,
                const matReal m20, const matReal m21, const matReal m22, const matReal m23,
                const matReal m30, const matReal m31, const matReal m32, const matReal m33);
            void reset();
            _mat4 operator * (const _mat4& p) const;
            template<class T>
            _vec4<T> operator* (const _vec4<T>& vec) const;
            _mat4 operator+ (const _mat4& p) const;
            _mat4 operator- (const _mat4& p) const;
            _mat4 operator* (const matReal n) const;
            _mat4 operator / (const matReal n) const;
            _mat4 operator+ (const matReal n) const;
            _mat4 operator- (const matReal n) const;
            void operator *= (const _mat4& p);
            void operator *= (const matReal n);
            void operator /= (const matReal n);
            void operator += (const _mat4& p);
            void operator += (const matReal n);
            void operator  -= (const _mat4& p);
            void operator  -= (const matReal n);
            bool operator == (const _mat4& p) const;
            bool operator != (const _mat4& p) const;

            inline _mat4& initRotation(matReal angle, const _vec3<matReal>& axis);
            inline _mat4& initTranslation(_vec3<matReal>& trans);
            inline _mat4& initScale(_vec3<matReal>& scale);

            _mat4& initRotation(matReal angle, const matReal x,
                const matReal y, const matReal z);
            _mat4& initTranslation(const matReal x, const matReal y, const matReal z);
            _mat4& initScale(const matReal x, const matReal y, const matReal z);

            _mat4& initProjection(const matReal fov, const matReal width,
                const matReal height, const matReal znear, const matReal zfar);
            _mat4& initOrthoProjection(const matReal left_, const matReal right_,
                const matReal bottom_, const matReal top_, const matReal near_,
                const matReal far_);

            _mat4 getTranspose();
        };

        class _mat3
        {
        public:
            matReal m[3][3];

            _mat3();
            explicit _mat3(const matReal n);
            _mat3(const matReal m00, const matReal m01, const matReal m02,
                const matReal m10, const matReal m11, const matReal m12,
                const matReal m20, const matReal m21, const matReal m22);
            void reset();
            _mat3 operator * (const _mat3& p) const;
            _vec3<matReal> operator* (const _vec3<matReal>& vec) const;
            _mat3 operator* (const matReal n) const;
            _mat3 operator+ (const _mat3& p) const;
            _mat3 operator- (const _mat3& p) const;
            _mat3 operator / (const matReal n) const;
            _mat3 operator+ (const matReal n) const;
            _mat3 operator- (const matReal n) const;
            void operator *= (const _mat3& p);
            void operator *= (const matReal n);
            void operator /= (const matReal n);
            void operator += (const _mat3& p);
            void operator += (const matReal n);
            void operator  -= (const _mat3& p);
            void operator  -= (const matReal n);
            bool operator == (const _mat3& p) const;
            bool operator != (const _mat3& p) const;

            _mat3 getTranspose() const;
            matReal det() const;
           _mat3 getInverse() const;
        };

        class _mat2
        {
        public:
            matReal m[2][2];

            _mat2();
            explicit _mat2(const matReal n);
            _mat2(const matReal m00, const matReal m01,
                const matReal m10, const matReal m11);
            void reset();
            _mat2 operator * (const _mat2& p) const;
            _vec2<matReal> operator* (const _vec2<matReal>& vec) const;
            _mat2 operator* (const matReal n) const;
            _mat2 operator+ (const _mat2& p) const;
            _mat2 operator- (const _mat2& p) const;
            _mat2 operator / (const matReal n) const;
            _mat2 operator+ (const matReal n) const;
            _mat2 operator- (const matReal n) const;
            void operator *= (const _mat2& p);
            void operator *= (const matReal n);
            void operator /= (const matReal n);
            void operator += (const _mat2& p);
            void operator += (const matReal n);
            void operator  -= (const _mat2& p);
            void operator  -= (const matReal n);
            bool operator == (const _mat2& p) const;
            bool operator != (const _mat2& p) const;

            _mat2 getTranspose();
        };

    }

    //Type determined through typedefs:
    typedef tmp::_vec2<matReal> vec2;
    typedef tmp::_vec3<matReal> vec3;
    typedef tmp::_vec4<matReal> vec4;
    typedef tmp::_vec2<matInt> vec2i;
    typedef tmp::_vec3<matInt> vec3i;
    typedef tmp::_vec4<matInt> vec4i;

    typedef tmp::_vec2<float> vec2f;
    typedef tmp::_vec3<float> vec3f;
    typedef tmp::_vec4<float> vec4f;
    typedef tmp::_vec2<double> vec2d;
    typedef tmp::_vec3<double> vec3d;
    typedef tmp::_vec4<double> vec4d;

    typedef tmp::_vec2<int> vec2ii;
    typedef tmp::_vec3<int> vec3ii;
    typedef tmp::_vec4<int> vec4ii;
    typedef tmp::_vec2<long long> vec2l;
    typedef tmp::_vec3<long long> vec3l;
    typedef tmp::_vec4<long long> vec4l;

    typedef tmp::_vec2<short> vec2s;
    typedef tmp::_vec3<short> vec3s;
    typedef tmp::_vec4<short> vec4s;

    typedef tmp::_mat2 mat2;
    typedef tmp::_mat3 mat3;
    typedef tmp::_mat4 mat4;

    typedef vec2 Vector2f;
    typedef vec3 Vector3f;
    typedef vec4 Vector4f;
    typedef vec2i Vector2i;
    typedef vec3i Vector3i;
    typedef vec4i Vector4i;
    typedef mat2 Matrix2x2;
    typedef mat3 Matrix3x3;
    typedef mat4 Matrix4x4;

    template <class T> T dot(const tmp::_vec2<T>& v1, const tmp::_vec2<T>& v2);
    template <class T> T dot(const tmp::_vec3<T>& v1, const tmp::_vec3<T>& v2);
    template <class T> T dot(const tmp::_vec4<T>& v1, const tmp::_vec4<T>& v2);
    template <class T> tmp::_vec3<T> cross(const tmp::_vec3<T>& v1, const tmp::_vec3<T>& v2);
}
