#include "ofxMSAFluidSolver.h"
#include "Particle.h"
#include "ParticleSystem.h"

#include "FluidSimField.h"

FluidSimField::FluidSimField(float fluidSize)
{
    _fluidSolver = new ofxMSAFluidSolver();

    _fluidSolver->setup();
    _fluidSolver->setSize(fluidSize, fluidSize)
                 .enableRGB(false)
                 .setFadeSpeed(0)
                 .setDeltaT(0.1)
                 .setVisc(0.005)
                 .setWrap(true, true)
                 .setColorDiffusion(0);
}

FluidSimField::~FluidSimField()
{
    delete _fluidSolver;
}

void FluidSimField::update(double dt)
{
    _fluidSolver->setDeltaT(dt);
    _fluidSolver->update();
}

void FluidSimField::addForceAtPos(float x, float y, float fx, float fy)
{
    _fluidSolver->addForceAtPos(x, y, fx, fy);
}

bool FluidSimField::_affectParticle(
    Particle *p,
    double dt,
    const ParticleSystem *particleSystem) const
{
    Vec3 fluidVel;
    Vec3 sysSize = particleSystem->getSize();

    _fluidSolver->getInfoAtPos(p->position.x / sysSize.x,
                               p->position.y / sysSize.y,
                               &fluidVel, NULL);

    // XXX figure out this fudge factor
    p->velocity += 24. * Vec3(fluidVel.x, fluidVel.y, 0.) * dt;

    return false;
}
