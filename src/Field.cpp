#include "Vec.h"
#include "VecField.h"

#include "ParticleSystem.h"

//// GravityField

bool GravityField::_affectParticle(
    Particle *p, double dt, const ParticleSystem * /* particleSystem */) const
{
    p->velocity += Vec3(0., -9.8 * dt, 0.);
    return false;
}

//// DeathPlaneField

DeathPlaneField::DeathPlaneField(float height) :
    _height(height)
{
}

bool DeathPlaneField::_affectParticle(
    Particle *p, double dt, const ParticleSystem *particleSystem) const
{
    return p->position.z < _height;
}

//// AdvectionField

bool AdvectionField::_affectParticle(
    Particle *p, double dt, const ParticleSystem *particleSystem) const
{
    const Vec3 &fieldSize = _vecField->getSize();

    float xpos = CLAMP(p->position.x, 0, fieldSize.x - 1.),
          ypos = CLAMP(p->position.y, 0, fieldSize.y - 1.),
          zpos = CLAMP(p->position.z, 0, fieldSize.z - 1.);

    const Vec3 &fieldVel = (*_vecField)(xpos, ypos, zpos);

    const float factor = _blendFactor;

    p->velocity = (1. - factor)*p->velocity + factor*fieldVel;

    return false;
}
