#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <vector>

#include "Particle.h"

class ParticleSystem
{
public:
    ParticleSystem(int size = 5000,
                   double maxVelocity = 0.1,
                   double friction = 0.1);

    virtual ~ParticleSystem();

    virtual void update(double dt);
    virtual void reset();

    virtual std::vector<Particle *> getParticles() { return _particles; }

private:
    int _size;
    double _maxVelocity;
    double _friction;

    std::vector<Particle *> _particles;

    virtual void _init();
    virtual void _destroy();
};

#endif /* PARTICLESYSTEM_H_ */
