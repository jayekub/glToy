#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <vector>

#include "Particle.h"

class ParticleSystem
{
public:
    ParticleSystem(int size = 5000, double maxVelocity = 0.1);
    virtual ~ParticleSystem();

    virtual void update(double dt);
    virtual std::vector<Particle *> getParticles() { return _particles; }

private:
    int _size;
    double _maxVelocity;

    std::vector<Particle *> _particles;

    virtual void _init();
};

#endif /* PARTICLESYSTEM_H_ */
