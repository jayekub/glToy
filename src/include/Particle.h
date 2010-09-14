#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Vec.h"

struct EmptyAttributes {
};

template<class A = EmptyAttributes>
struct Particle
{
	Vec3 position;
	Vec3 velocity;
	A attributes;
};

#endif /* PARTICLE_H_ */
