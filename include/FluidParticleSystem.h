#ifndef FLUIDPARTICLESYSTEM_H_
#define FLUIDPARTICLESYSTEM_H_

#include "ofxMSAFluidSolver.h"
#include "ParticleSystem.h"

class FluidParticleSystem : public ParticleSystem
{
public:
    FluidParticleSystem() {};
    FluidParticleSystem(int num, double maxVelocity, double friction) :
        ParticleSystem(num, maxVelocity, friction) {}

    virtual void update(double dt, ofxMSAFluidSolver *fluid);
};

#endif /* FLUIDPARTICLESYSTEM_H_ */
