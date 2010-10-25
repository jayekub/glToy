#ifndef __FLUIDSIMFIELD_H__
#define __FLUIDSIMFIELD_H__

#include "Vec.h"
#include "Field.h"

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

    bool _affectParticle(Particle *p, double dt,
        const ParticleSystem *particleSystem) const;
};

#endif // __FLUIDSIMFIELD_H__
