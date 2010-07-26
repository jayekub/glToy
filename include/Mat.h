#ifndef MAT_H_
#define MAT_H_

#include "Vec.h"

// stored column major
class Mat4x4
{
public:
    union {
        vec_t v[16];
        vec_t m[4][4]; // COLUMN MAJOR!!
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
        float det = determinant();

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

    Mat4x4 operator*=(const float &a) {
        Mat4x4 old = *this;

        for (int e = 0; e < 16; ++e) {
            v[e] *= a;
        }

        return old;
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

    Mat4x4() {
        // initialize to the identity
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[j][i] = i == j ? 1. : 0.;
    };

    Mat4x4(float m11, float m12, float m13, float m14,
           float m21, float m22, float m23, float m24,
           float m31, float m32, float m33, float m34,
           float m41, float m42, float m43, float m44) {

        v[0] = m11; v[1] = m21; v[2] = m31; v[3] = m41;
        v[4] = m12; v[5] = m22; v[6] = m32; v[7] = m42;
        v[8] = m13; v[9] = m23; v[10] = m33; v[11] = m43;
        v[12] = m14; v[13] = m24; v[14] = m34; v[15] = m44;
    };
};

#endif // MAT_H_
