#include "ofxMSAFluidSolver.h"
#include "Particle.h"

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

Vec3 FluidSimField::_particleAcceleration(
    double dt,
    Particle *p,
    const ParticleSystem *particleSystem)
{
    // XXX get rid of this stupid class
    ofPoint fluidVel;
    Vec3 sysSize = particleSystem->getSize();

    _fluidSolver->getInfoAtPos(p->position.x / sysSize.x,
                               p->position.y / sysSize.y,
                               &fluidVel, NULL);

    return Vec3(24. * fluidVel.x, 24. * fluidVel.y, 0.);
    //return Vec3(0., 0., 0.);
}
