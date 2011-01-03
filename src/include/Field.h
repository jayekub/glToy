#ifndef __FIELD_H__
#define __FIELD_H__

#include "Particle.h"

class ParticleSystem;
class VecField;

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

class DeathPlaneField : public Field
{
public:
    DeathPlaneField(float height);

protected:
    float _height;

    bool _affectParticle(Particle *p, double dt,
                         const ParticleSystem *particleSystem) const;
};

class AdvectionField : public Field
{
public:
    AdvectionField(const VecField *vecField, float blendFactor) :
        _vecField(vecField), _blendFactor(blendFactor) {}

protected:
    const VecField *_vecField;
    float _blendFactor;

    bool _affectParticle(Particle *p, double dt,
                         const ParticleSystem * /* particleSystem */) const;

};

#endif // __FIELD_H__

