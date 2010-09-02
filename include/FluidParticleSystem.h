#ifndef FLUIDPARTICLESYSTEM_H_
#define FLUIDPARTICLESYSTEM_H_

#include "ofxMSAFluidSolver.h"
#include "ParticleSystem.h"

class FluidParticleSystem : public ParticleSystem<Vec2>
{
public:
    FluidParticleSystem(const char *name, const Vec2 &size) :
        ParticleSystem<Vec2>(name, size) {}

    virtual void update(double dt, ofxMSAFluidSolver *fluid) {
        BOOST_FOREACH(Particle<Vec2> *p, _particles) {
            ofPoint velocity;

            fluid->getInfoAtPos(p->position.x, p->position.y, &velocity, NULL);

            p->position.x += velocity.x * dt * 24.0;
            p->position.y += velocity.y * dt * 24.0;

            _wrapParticle(p);
        }
    }
};

#endif /* FLUIDPARTICLESYSTEM_H_ */
