#ifndef VEC_H_
#define VEC_H_

#include <math.h>

struct Vec2 {
    union {
        struct {
            float x;
            float y;
        };
        float v[2];
    };

    Vec2() {
    }

    Vec2(double x_, double y_) :
        x(x_), y(y_) {
    }

    double length() {
        return sqrt(x * x + y * y);
    }

    Vec2 mult(float a) {
        return Vec2(a * x, a * y);
    }

    Vec2 normalize() {
        return mult(1.0 / length());
    }

    Vec2 operator+(const Vec2 &v) {
        return Vec2(x - v.x, y - v.y);
    }

    Vec2 operator-(const Vec2 &v) {
        return Vec2(x - v.x, y - v.y);
    }
};

struct Vec4 {
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        struct {
            float r;
            float g;
            float b;
            float a;
        };
        float v[4];
    };

    Vec4() {
    }

    Vec4(double x_, double y_, double z_, double w_) :
        x(x_), y(y_), z(z_), w(w_) {
    }

    float length() {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    Vec4 mult(float a) {
        return Vec4(a * x, a * y, a * z, a * w);
    }

    Vec4 normalize() {
        return mult(1.0 / length());
    }

    Vec4 operator+(const Vec4 &v) {
        return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    Vec4 operator-(const Vec4 &v) {
        return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
    }
};

#endif /* VEC_H_ */
