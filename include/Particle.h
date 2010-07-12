#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <math.h>

struct Vec2d {
	double x;
	double y;

	Vec2d() {
	}

	Vec2d(double x_, double y_) :
		x(x_), y(y_) {
	}

	double length() {
		return sqrt(x * x + y * y);
	}

	Vec2d mult(double a) {
		return Vec2d(a * x, a * y);
	}

	Vec2d normalize() {
		return mult(1.0 / length());
	}

	Vec2d operator+(const Vec2d &v) {
	    return Vec2d(x - v.x, y - v.y);
	}

	Vec2d operator-(const Vec2d &v) {
	    return Vec2d(x - v.x, y - v.y);
	}
};

class Particle {
public:
	Vec2d position, velocity;
};

#endif /* PARTICLE_H_ */
