#include "Particle.h"
#include "ParticleSystem.h"

#include "Emitter.h"

//// RandomEmitter

RandomEmitter::RandomEmitter() :
    _numToEmit(0), _meanSpeed(0.), _meanRadius(0.)
{
}

void RandomEmitter::emitOnce(
    int numParticles, float meanSpeed, float meanRadius)
{
    _numToEmit = numParticles;
    _meanSpeed = meanSpeed;
    _meanRadius = meanRadius;
}

void RandomEmitter::_emitParticles(
    double dt, const ParticleSystem *particleSystem,
    std::vector<Particle *> &particles)
{
    for (int i = 0; i < _numToEmit; ++i) {
        Particle *p = new Particle();
        const Vec3 ps = particleSystem->getSize();

        p->position =
            Vec3(randFloat()*ps.x,
                 randFloat()*ps.y,
                 randFloat()*ps.z);

        p->velocity = _meanSpeed*(randFloat() + 0.5)*Vec3::randVec(-1, 1).normalize();

        p->radius = _meanRadius*(randFloat() + 0.5);

        particles.push_back(p);
    }

    _numToEmit = 0;
}

//// SprayEmitter

// TODO rename PointEmitter

void SprayEmitter::_emitParticles(
    double dt, const ParticleSystem *particleSystem,
    std::vector<Particle *> &particles)
{
    int numToEmit = round(_rate*dt + 0.5*randFloat());

    for (int i = 0; i < numToEmit; ++i) {
        const float speed = _speedMean + (randFloat() - 0.5)*_speedSpread;
        const float radius = _radMean + (randFloat() - 0.5)*_radSpread;

        const Vec3 randAxis = Vec3::randVec(-1, 1).normalize();
        const Vec3 dir = _dirMean.rotate(
            (2.*randFloat() - 1.)*_dirSpread, randAxis);

        Particle *p = new Particle();

        p->position = _position;
        p->velocity = speed*dir.normalize();
        p->radius = radius;

        particles.push_back(p);
    }
}
