#ifndef __FIELD_H__
#define __FIELD_H__

#include "Vec.h"
#include "Particle.h"

class ParticleSystem;

class Field
{
public:
    virtual void update(double dt) {};

protected:
    friend class ParticleSystem;

    // if true, kill this particle
    virtual bool _affectParticle(Particle *p, double dt,
        const ParticleSystem *particleSystem) const = 0;
};

class GravityField : public Field
{
    bool _affectParticle(Particle *p, double dt,
                         const ParticleSystem * /* particleSystem */) const;
};

class BoundaryField : public Field
{
public:
    BoundaryField(float height);

protected:
    float _height;

    bool _affectParticle(Particle *p, double dt,
                         const ParticleSystem *particleSystem) const;
};

#endif // __FIELD_H__

