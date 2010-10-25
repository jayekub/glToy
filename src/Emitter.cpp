#include "Particle.h"
#include "ParticleSystem.h"

#include "Emitter.h"

//// RandomEmitter

RandomEmitter::RandomEmitter() :
    _numToEmit(0), _maxSpeed(0.), _meanRadius(0.)
{
}

void RandomEmitter::emitOnce(
    int numParticles, float maxSpeed, float meanRadius)
{
    _numToEmit = numParticles;
    _maxSpeed = maxSpeed;
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
            Vec3(randFloat() * ps.x,
                 randFloat() * ps.y,
                 randFloat() * ps.z);

        p->velocity = _maxSpeed * Vec3::randVec(-1, 1).normalize();

        p->radius = _meanRadius * (randFloat() + 0.5);

        particles.push_back(p);
    }

    _numToEmit = 0;
}

//// SprayEmitter

SprayEmitter::SprayEmitter(
    float rate, Vec3 position,
    Vec3 dirMean, float dirSpread,
    float radMean, float radSpread,
    float speedMean, float speedSpread) :
    _rate(rate), _position(_position),
    _dirMean(dirMean), _dirSpread(dirSpread),
    _radMean(radMean), _radSpread(radSpread),
    _speedMean(speedMean), _speedSpread(speedSpread)
{
    _A = _dirMean.rotate(M_PI / 2., Vec3(0., 1., 0.)).normalize();
    _B = _dirMean.rotate(M_PI / 2., Vec3(1., 0., 0.)).normalize();
}

void SprayEmitter::_emitParticles(
    double dt, const ParticleSystem *particleSystem,
    std::vector<Particle *> &particles)
{
    int numToEmit = floor(_rate * dt);

    for (int i = 0; i < numToEmit; ++i) {
        float speed = _speedMean + (randFloat() - 0.5) * _speedSpread;
        float radius = _radMean + (randFloat() - 0.5) * _radSpread;
        Vec3 off = sin(_dirSpread) * 
            ((2. * randFloat() - 1.) * _A +
             (2. * randFloat() - 1.) * _B).normalize();

        Particle *p = new Particle();

        p->position = _position;
        p->velocity = speed * (_dirMean + off).normalize();
        p->radius = radius;

        particles.push_back(p);
    }
}
