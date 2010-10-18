#ifndef __FLUIDSIMFIELD_H__
#define __FLUIDSIMFIELD_H__

#include "Vec.h"
#include "ParticleSystem.h"

class Particle;
class ofxMSAFluidSolver;

class FluidSimField : public Field
{
public:
    FluidSimField(float fluidSize);
    ~FluidSimField();

    void update(double dt);

    void addForceAtPos(float x, float y, float fx, float fy);

protected:
    ofxMSAFluidSolver *_fluidSolver;

    Vec3 _particleAcceleration(double dt, Particle *p,
        const ParticleSystem *particleSystem);
};

#endif // __FLUIDSIMFIELD_H__
