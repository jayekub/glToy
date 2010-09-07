#ifndef FLUIDPARTICLESYSTEM_H_
#define FLUIDPARTICLESYSTEM_H_

#include "ofxMSAFluidSolver.h"
#include "ParticleSystem.h"

// XXX refactor as a Field
class FluidParticleSystem : public ParticleSystem<>
{
public:
    FluidParticleSystem(const char *name, const Vec2 &size) :
        ParticleSystem<>(name, Vec3(size, 0.)) {}

    virtual void update(double dt, ofxMSAFluidSolver *fluid) {
        BOOST_FOREACH(Particle<> *p, _particles) {
            ofPoint velocity;

            fluid->getInfoAtPos(p->position.x, p->position.y, &velocity, NULL);

            p->position.x += velocity.x * dt * 24.0;
            p->position.y += velocity.y * dt * 24.0;

            //_wrapParticle(p);
        }
    }
};

#endif /* FLUIDPARTICLESYSTEM_H_ */
