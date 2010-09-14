#ifndef MAT_H_
#define MAT_H_

#include <math.h>

#include <boost/format.hpp>

#include "Vec.h"

// stored column major
class Mat4x4
{
public:
    union {
        vec_t v[16];
        vec_t m[4][4]; // COLUMN MAJOR!!
    };

    Mat4x4() {
        // initialize to all zeroes
        for (int e = 0; e < 16; ++e)
            v[e] = 0;
    };

    Mat4x4(vec_t m11, vec_t m12, vec_t m13, vec_t m14,
           vec_t m21, vec_t m22, vec_t m23, vec_t m24,
           vec_t m31, vec_t m32, vec_t m33, vec_t m34,
           vec_t m41, vec_t m42, vec_t m43, vec_t m44) {

        v[0] = m11; v[1] = m21; v[2] = m31; v[3] = m41;
        v[4] = m12; v[5] = m22; v[6] = m32; v[7] = m42;
        v[8] = m13; v[9] = m23; v[10] = m33; v[11] = m43;
        v[12] = m14; v[13] = m24; v[14] = m34; v[15] = m44;
    };

    const vec_t &operator()(int i, int j) const {
        return m[j][i];
    }

    vec_t &operator()(int i, int j) {
        return m[j][i];
    }

    float determinant() const {
        const Mat4x4 &me = *this;
        
        return
            me(0, 3)*me(1, 2)*me(2, 1)*me(3, 0) - me(0, 2)*me(1, 3)*me(2, 1)*me(3, 0) - me(0, 3)*me(1, 1)*me(2, 2)*me(3, 0) + me(0, 1)*me(1, 3)*me(2, 2)*me(3, 0) +
            me(0, 2)*me(1, 1)*me(2, 3)*me(3, 0) - me(0, 1)*me(1, 2)*me(2, 3)*me(3, 0) - me(0, 3)*me(1, 2)*me(2, 0)*me(3, 1) + me(0, 2)*me(1, 3)*me(2, 0)*me(3, 1) +
            me(0, 3)*me(1, 0)*me(2, 2)*me(3, 1) - me(0, 0)*me(1, 3)*me(2, 2)*me(3, 1) - me(0, 2)*me(1, 0)*me(2, 3)*me(3, 1) + me(0, 0)*me(1, 2)*me(2, 3)*me(3, 1) +
            me(0, 3)*me(1, 1)*me(2, 0)*me(3, 2) - me(0, 1)*me(1, 3)*me(2, 0)*me(3, 2) - me(0, 3)*me(1, 0)*me(2, 1)*me(3, 2) + me(0, 0)*me(1, 3)*me(2, 1)*me(3, 2) +
            me(0, 1)*me(1, 0)*me(2, 3)*me(3, 2) - me(0, 0)*me(1, 1)*me(2, 3)*me(3, 2) - me(0, 2)*me(1, 1)*me(2, 0)*me(3, 3) + me(0, 1)*me(1, 2)*me(2, 0)*me(3, 3) +
            me(0, 2)*me(1, 0)*me(2, 1)*me(3, 3) - me(0, 0)*me(1, 2)*me(2, 1)*me(3, 3) - me(0, 1)*me(1, 0)*me(2, 2)*me(3, 3) + me(0, 0)*me(1, 1)*me(2, 2)*me(3, 3);
    }

    Mat4x4 transpose() const {
        Mat4x4 t;

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                t.m[j][i] = m[i][j];

        return t;
    }

    Mat4x4 inverse() const {
        const Mat4x4 &me = *this;
        Mat4x4 mI;
        vec_t det = determinant();

        if (det == 0.0) return mI;

        mI(0, 0) = me(1, 2)*me(2, 3)*me(3, 1) - me(1, 3)*me(2, 2)*me(3, 1) + me(1, 3)*me(2, 1)*me(3, 2) - me(1, 1)*me(2, 3)*me(3, 2) - me(1, 2)*me(2, 1)*me(3, 3) + me(1, 1)*me(2, 2)*me(3, 3);
        mI(0, 1) = me(0, 3)*me(2, 2)*me(3, 1) - me(0, 2)*me(2, 3)*me(3, 1) - me(0, 3)*me(2, 1)*me(3, 2) + me(0, 1)*me(2, 3)*me(3, 2) + me(0, 2)*me(2, 1)*me(3, 3) - me(0, 1)*me(2, 2)*me(3, 3);
        mI(0, 2) = me(0, 2)*me(1, 3)*me(3, 1) - me(0, 3)*me(1, 2)*me(3, 1) + me(0, 3)*me(1, 1)*me(3, 2) - me(0, 1)*me(1, 3)*me(3, 2) - me(0, 2)*me(1, 1)*me(3, 3) + me(0, 1)*me(1, 2)*me(3, 3);
        mI(0, 3) = me(0, 3)*me(1, 2)*me(2, 1) - me(0, 2)*me(1, 3)*me(2, 1) - me(0, 3)*me(1, 1)*me(2, 2) + me(0, 1)*me(1, 3)*me(2, 2) + me(0, 2)*me(1, 1)*me(2, 3) - me(0, 1)*me(1, 2)*me(2, 3);

        mI(1, 0) = me(1, 3)*me(2, 2)*me(3, 0) - me(1, 2)*me(2, 3)*me(3, 0) - me(1, 3)*me(2, 0)*me(3, 2) + me(1, 0)*me(2, 3)*me(3, 2) + me(1, 2)*me(2, 0)*me(3, 3) - me(1, 0)*me(2, 2)*me(3, 3);
        mI(1, 1) = me(0, 2)*me(2, 3)*me(3, 0) - me(0, 3)*me(2, 2)*me(3, 0) + me(0, 3)*me(2, 0)*me(3, 2) - me(0, 0)*me(2, 3)*me(3, 2) - me(0, 2)*me(2, 0)*me(3, 3) + me(0, 0)*me(2, 2)*me(3, 3);
        mI(1, 2) = me(0, 3)*me(1, 2)*me(3, 0) - me(0, 2)*me(1, 3)*me(3, 0) - me(0, 3)*me(1, 0)*me(3, 2) + me(0, 0)*me(1, 3)*me(3, 2) + me(0, 2)*me(1, 0)*me(3, 3) - me(0, 0)*me(1, 2)*me(3, 3);
        mI(1, 3) = me(0, 2)*me(1, 3)*me(2, 0) - me(0, 3)*me(1, 2)*me(2, 0) + me(0, 3)*me(1, 0)*me(2, 2) - me(0, 0)*me(1, 3)*me(2, 2) - me(0, 2)*me(1, 0)*me(2, 3) + me(0, 0)*me(1, 2)*me(2, 3);

        mI(2, 0) = me(1, 1)*me(2, 3)*me(3, 0) - me(1, 3)*me(2, 1)*me(3, 0) + me(1, 3)*me(2, 0)*me(3, 1) - me(1, 0)*me(2, 3)*me(3, 1) - me(1, 1)*me(2, 0)*me(3, 3) + me(1, 0)*me(2, 1)*me(3, 3);
        mI(2, 1) = me(0, 3)*me(2, 1)*me(3, 0) - me(0, 1)*me(2, 3)*me(3, 0) - me(0, 3)*me(2, 0)*me(3, 1) + me(0, 0)*me(2, 3)*me(3, 1) + me(0, 1)*me(2, 0)*me(3, 3) - me(0, 0)*me(2, 1)*me(3, 3);
        mI(2, 2) = me(0, 1)*me(1, 3)*me(3, 0) - me(0, 3)*me(1, 1)*me(3, 0) + me(0, 3)*me(1, 0)*me(3, 1) - me(0, 0)*me(1, 3)*me(3, 1) - me(0, 1)*me(1, 0)*me(3, 3) + me(0, 0)*me(1, 1)*me(3, 3);
        mI(2, 3) = me(0, 3)*me(1, 1)*me(2, 0) - me(0, 1)*me(1, 3)*me(2, 0) - me(0, 3)*me(1, 0)*me(2, 1) + me(0, 0)*me(1, 3)*me(2, 1) + me(0, 1)*me(1, 0)*me(2, 3) - me(0, 0)*me(1, 1)*me(2, 3);

        mI(3, 0) = me(1, 2)*me(2, 1)*me(3, 0) - me(1, 1)*me(2, 2)*me(3, 0) - me(1, 2)*me(2, 0)*me(3, 1) + me(1, 0)*me(2, 2)*me(3, 1) + me(1, 1)*me(2, 0)*me(3, 2) - me(1, 0)*me(2, 1)*me(3, 2);
        mI(3, 1) = me(0, 1)*me(2, 2)*me(3, 0) - me(0, 2)*me(2, 1)*me(3, 0) + me(0, 2)*me(2, 0)*me(3, 1) - me(0, 0)*me(2, 2)*me(3, 1) - me(0, 1)*me(2, 0)*me(3, 2) + me(0, 0)*me(2, 1)*me(3, 2);
        mI(3, 2) = me(0, 2)*me(1, 1)*me(3, 0) - me(0, 1)*me(1, 2)*me(3, 0) - me(0, 2)*me(1, 0)*me(3, 1) + me(0, 0)*me(1, 2)*me(3, 1) + me(0, 1)*me(1, 0)*me(3, 2) - me(0, 0)*me(1, 1)*me(3, 2);
        mI(3, 3) = me(0, 1)*me(1, 2)*me(2, 0) - me(0, 2)*me(1, 1)*me(2, 0) + me(0, 2)*me(1, 0)*me(2, 1) - me(0, 0)*me(1, 2)*me(2, 1) - me(0, 1)*me(1, 0)*me(2, 2) + me(0, 0)*me(1, 1)*me(2, 2);

        mI *= 1. / det;

        return mI;
    }

    Vec3 ptransform(const Vec3 &vec) const {
        const Mat4x4 &me = *this;
        return Vec3(
            me(0, 0) * vec.x + me(0, 1) * vec.y + me(0, 2) * vec.z + me(0, 3),
            me(1, 0) * vec.x + me(1, 1) * vec.y + me(1, 2) * vec.z + me(1, 3),
            me(2, 0) * vec.x + me(2, 1) * vec.y + me(2, 2) * vec.z + me(2, 3));
    }

    Vec3 getTranslation() const {
        const Mat4x4 &me = *this;
        return Vec3(me(0, 3), me(1, 3), me(2, 3));
    }

    // XXX add getRotation and getScale?

    Mat4x4 operator*=(const vec_t &a) {
        for (int e = 0; e < 16; ++e) {
            v[e] *= a;
        }

        return *this;
    }

    Mat4x4 operator*=(const Mat4x4 &mat) {
        *this = *this * mat;
        return *this;
    }

    Mat4x4 operator*(const Mat4x4 &mat) const {
        const Mat4x4 &me = *this;
        Mat4x4 r;


#define COMPUTE(i, j) r(i, j) = me(i, 0) * mat(0, j) + \
                                me(i, 1) * mat(1, j) + \
                                me(i, 2) * mat(2, j) + \
                                me(i, 3) * mat(3, j);

        COMPUTE(0, 0); COMPUTE(0, 1); COMPUTE(0, 2); COMPUTE(0, 3);
        COMPUTE(1, 0); COMPUTE(1, 1); COMPUTE(1, 2); COMPUTE(1, 3);
        COMPUTE(2, 0); COMPUTE(2, 1); COMPUTE(2, 2); COMPUTE(2, 3);
        COMPUTE(3, 0); COMPUTE(3, 1); COMPUTE(3, 2); COMPUTE(3, 3);

#undef COMPUTE

        return r;
    }

    std::string toString() const {
        boost::format matFmt("[[%1%, %2%, %3%, %4%], "
                             "[%5%, %6%, %7%, %8%], "
                             "[%9%, %10%, %11%, %12%], "
                             "[%13%, %14%, %15%, %16%]]");

        matFmt = matFmt % v[0] % v[4] % v[8] % v[12]
                        % v[1] % v[5] % v[9] % v[13]
                        % v[2] % v[6] % v[10] % v[14]
                        % v[3] % v[7] % v[11] % v[15];

        return matFmt.str();
    }

    static Mat4x4 identity() {
        Mat4x4 I;

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                I(i, j) = i == j ? 1. : 0.;

        return I;
    }

    static Mat4x4 translate(const Vec3 &t) {
        Mat4x4 T = Mat4x4::identity();

        T(0, 3) = t.x;
        T(1, 3) = t.y;
        T(2, 3) = t.z;

        return T;
    }

    // angle is in degrees
    static Mat4x4 rotate(vec_t angle, const Vec3 &axis) {
        Mat4x4 R = Mat4x4::identity();
        Vec3 A = axis.normalize();

        vec_t c = cos(angle);
        vec_t omc = 1. - c;
        vec_t s = sin(angle);

        vec_t &x = A.x;
        vec_t &y = A.y;
        vec_t &z = A.z;

        R(0, 0) = x * x * omc + c;
        R(0, 1) = x * y * omc - z * s;
        R(0, 2) = x * z * omc + y * s;
        //R(0, 3) = 0.;

        R(1, 0) = y * x * omc + z * s;
        R(1, 1) = y * y * omc + c;
        R(1, 2) = y * z * omc - x * s;
        //R(1, 3) = 0.;

        R(2, 0) = x * z * omc - y * s;
        R(2, 1) = y * z * omc + x * s;
        R(2, 2) = z * z * omc + c;
        //R(2, 3) = 0.;

        //R(3, 0) = 0.;
        //R(3, 1) = 0.;
        //R(3, 2) = 0.;
        //R(3, 3) = 1.;

        return R;
    }

    static Mat4x4 scale(const Vec3 &s) {
        Mat4x4 S = Mat4x4::identity();

        S(0, 0) = s.x;
        S(1, 1) = s.y;
        S(2, 2) = s.z;

        return S;
    }

    static Mat4x4 ortho(vec_t left, vec_t right, vec_t bottom, vec_t top,
                        vec_t nearVal, vec_t farVal) {

        Vec3 T(-(right + left) / (right - left),
               -(top + bottom) / (top - bottom),
               -(farVal + nearVal) / (farVal - nearVal));

        Mat4x4 O = translate(T);

        O(0, 0) = 2. / (right - left);
        O(1, 1) = 2. / (top - bottom);
        O(2, 2) = -2. / (farVal - nearVal);

        return O;
    }

    static Mat4x4 perspective(vec_t fovy, vec_t aspect,
                              vec_t zNear, vec_t zFar) {

        vec_t f = 1. / tan(fovy / 2.);

        Mat4x4 P;

        P(0, 0) = f / aspect;
        P(1, 1) = f;
        P(2, 2) = (zFar + zNear) / (zNear - zFar);
        P(2, 3) = 2. * (zFar * zNear) / (zNear - zFar);
        P(3, 2) = -1.;
        P(3, 3) = 0.;

        return P;
    }

    static Mat4x4 lookat(const Vec3 &eye, const Vec3 &center, const Vec3 &up) {
        Vec3 f = (center - eye).normalize();
        Vec3 UPn = up.normalize();

        Vec3 s = f.cross(UPn).normalize();
        Vec3 u = s.cross(f).normalize();

        Mat4x4 M(s.x, s.y, s.z, 0.,
                 u.x, u.y, u.z, 0.,
                 -f.x, -f.y, -f.z, 0.,
                 0., 0., 0., 1.);

        Mat4x4 T = translate(-1. * eye);

        return M * T;
    }
};

typedef Mat4x4 Mat4;

#endif // MAT_H_
