#ifndef VEC_H_
#define VEC_H_

#include <cmath>
#include <string>

#include <boost/format.hpp>

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

    Vec2(vec_t x_, vec_t y_) :
        x(x_), y(y_) {
    }

    vec_t length() const {
        return std::sqrt(x * x + y * y);
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

    std::string toString() const {
        boost::format vecFmt("[%1%, %2%]");

        vecFmt = vecFmt % v[0] % v[1];
        return vecFmt.str();
    }

    static Vec2 randVec(vec_t min = 0., vec_t max = 1.) {
        vec_t range = max - min;
        return Vec2(range * randFloat() + min,
                    range * randFloat() + min);
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

    Vec3() : x(0), y(0), z(0) {}

    Vec3(vec_t x_, vec_t y_, vec_t z_) :
        x(x_), y(y_), z(z_) {}

    Vec3(const Vec2 &vec, vec_t z_) :
        x(vec.x), y(vec.y), z(z_) {}

    vec_t length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec3 normalize() const {
        return *this * (1.0 / length());
    }

    vec_t dot(const Vec3 &vec) const {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    Vec3 cross(const Vec3 &vec) const {
        return Vec3(y * vec.z - vec.y * z,
                    z * vec.x - vec.z * x,
                    x * vec.y - vec.x * y);
    }

    Vec3 rotate(const float &angle, const Vec3 &axis) const {
        Vec3 P = cross(axis);

        return P.length() == 0 ? *this :
            ((*this) * std::cos(angle) + P.normalize() * std::sin(angle));
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

    Vec3 &operator-=(const Vec3 &vec) {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;

        return *this;
    }

    void set(const vec_t &x_, const vec_t &y_, const vec_t &z_ = 0.) {
        x = x_; y = y_; z = z_;
    }

    std::string toString() const {
        boost::format vecFmt("[%1%, %2%, %3%]");

        vecFmt = vecFmt % v[0] % v[1] % v[2];
        return vecFmt.str();
    }

    static Vec3 randVec(vec_t min = 0., vec_t max = 1.) {
        vec_t range = max - min;
        return Vec3(range * randFloat() + min,
                    range * randFloat() + min,
                    range * randFloat() + min);
    }
};

inline Vec3 operator*(const vec_t &a, const Vec3 &v) {
    return v * a;
}

#endif /* VEC_H_ */
