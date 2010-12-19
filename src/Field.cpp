#include "ParticleSystem.h"

//// GravityField

bool GravityField::_affectParticle(
    Particle *p, double dt, const ParticleSystem * /* particleSystem */) const
{
    p->velocity += Vec3(0., -9.8 * dt, 0.);
    return false;
}

//// BoundaryField

BoundaryField::BoundaryField(float height) :
    _height(height)
{
}

bool BoundaryField::_affectParticle(
    Particle *p, double dt, const ParticleSystem *particleSystem) const
{
    return p->position.z < _height;
}

