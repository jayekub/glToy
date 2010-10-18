#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Vec.h"

struct Particle
{
    Vec3 position;
    Vec3 velocity;

    float radius;
};

#endif /* PARTICLE_H_ */
