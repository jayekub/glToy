#ifndef MAT_H_
#define MAT_H_

#include <math.h>

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
        // initialize to the identity
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[j][i] = i == j ? 1. : 0.;
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

        mI *= det;

        return mI;
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

    static Mat4x4 translate(const Vec3 &t) {
        Mat4x4 T;

        T(0, 3) = t.x;
        T(1, 3) = t.y;
        T(2, 3) = t.z;

        return T;
    }

    // angle is in degrees
    static Mat4x4 rotate(vec_t angle, const Vec3 &axis) {
        Mat4x4 R;
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
        Mat4x4 S;

        S(0, 0) = s.x;
        S(1, 1) = s.y;
        S(2, 2) = s.z;

        return S;
    }

    // XXX add lookat, ortho, and perspective
};

typedef Mat4x4 Mat4;

#endif // MAT_H_
