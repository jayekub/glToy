#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <vector>

#include "Particle.h"

#define WRAP_PARTICLE(p) \
{ \
        if (p->position.x < 0.0) \
            p->position.x += 1.0; \
        else if (p->position.x > 1.0) \
            p->position.x -= 1.0; \
\
        if (p->position.y < 0.0) \
            p->position.y += 1.0; \
        else if (p->position.y > 1.0) \
            p->position.y -= 1.0; \
}

class ParticleSystem
{
public:
    ParticleSystem() : _num(0), _maxVelocity(0.0), _friction(0.0) {};
    ParticleSystem(int num, double maxVelocity, double friction);

    virtual ~ParticleSystem();

    virtual void addRandom(int num, double maxVelocity);
    virtual void update(double dt);
    virtual void reset();

    virtual std::vector<Particle *> getParticles() { return _particles; }

protected:
    int _num;
    double _maxVelocity;
    double _friction;

    std::vector<Particle *> _particles;

    virtual void _destroy();
};

#endif /* PARTICLESYSTEM_H_ */
