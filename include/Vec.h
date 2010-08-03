#ifndef VEC_H_
#define VEC_H_

#include <math.h>

#include "utils.h"

typedef float vec_t;

struct Vec2 {
    union {
        struct {
            vec_t x;
            vec_t y;
        };
        vec_t v[2];
    };

    Vec2() : x(0), y(0) {
    }

    Vec2(double x_, double y_) :
        x(x_), y(y_) {
    }

    vec_t length() const {
        return sqrt(x * x + y * y);
    }

    Vec2 normalize() const {
        return *this * (1. / length());
    }

    Vec2 operator+(const Vec2 &vec) const {
        return Vec2(x - vec.x, y - vec.y);
    }

    Vec2 operator-(const Vec2 &vec) const {
        return Vec2(x - vec.x, y - vec.y);
    }

    Vec2 operator*(const vec_t &a) const {
        return Vec2(a * x, a * y);
    }

    Vec2 &operator+=(const Vec2 &vec) {
        x += vec.x;
        y += vec.y;

        return *this;
    }
};

inline Vec2 operator*(const vec_t &a, const Vec2 &v) {
    return v * a;
}

struct Vec3 {
    union {
        struct {
            vec_t x;
            vec_t y;
            vec_t z;
        };
        struct {
            vec_t r;
            vec_t g;
            vec_t b;
        };
        vec_t v[3];
    };

    Vec3() : x(0), y(0), z(0) {
    }

    Vec3(double x_, double y_, double z_) :
        x(x_), y(y_), z(z_) {
    }

    vec_t length() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vec3 normalize() {
        return *this * (1.0 / length());
    }

    Vec3 operator+(const Vec3 &vec) const {
        return Vec3(x + vec.x, y + vec.y, z + vec.z);
    }

    Vec3 operator-(const Vec3 &vec) const {
        return Vec3(x - vec.x, y - vec.y, z - vec.z);
    }

    Vec3 operator*(const vec_t &a) const {
        return Vec3(a * x, a * y, a * z);
    }

    Vec3 &operator+=(const Vec3 &vec) {
        x += vec.x;
        y += vec.y;
        z += vec.z;

        return *this;
    }

    static Vec3 randVec(float min = 0., float max = 1.) {
        float range = max - min;
        return Vec3(range * randFloat() + min,
                    range * randFloat() + min,
                    range * randFloat() + min);
    }
};

inline Vec3 operator*(const vec_t &a, const Vec3 &v) {
    return v * a;
}

#endif /* VEC_H_ */
