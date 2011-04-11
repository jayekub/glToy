#ifndef __EMITTER_H__
#define __EMITTER_H__

#include <vector>

#include "ptr.h"

class Particle;
class ParticleSystem;

class Emitter
{
public:
    virtual void update(double dt) { };

protected:
    friend class ParticleSystem;

    virtual void _emitParticles(double dt,
        const ParticleSystem *particleSystem,
        std::vector<Particle *> &particles) = 0;
};

DEF_SHARED_PTR(RandomEmitter);

class RandomEmitter : public Emitter
{
public:
    RandomEmitter();

    void emitOnce(int numParticles, float meanSpeed, float meanRadius = 0.);

    DEF_CREATE(RandomEmitter);

private:
    int _numToEmit;
    float _meanSpeed;
    float _meanRadius;

    void _emitParticles(double dt, const ParticleSystem *particleSystem,
                        std::vector<Particle *> &particles);
};

class SprayEmitter : public Emitter
{
public:
    SprayEmitter() :
        _rate(0), _position(0, 0, 0), _dirMean(0, 1, 0), _dirSpread(0),
        _radMean(10), _radSpread(0), _speedMean(10), _speedSpread(0) {}

    SprayEmitter &setRate(float rate) { _rate = rate; return *this; }
    const float &getRate() const { return _rate; }

    SprayEmitter &setPosition(const Vec3 &position) {
        _position = position; return *this; }
    const Vec3 &getPosition() const { return _position; }

    SprayEmitter &setDir(const Vec3 &dir) { _dirMean = dir; return *this; }
    const Vec3 &getDir() const { return _dirMean; }

    SprayEmitter &setDirSpread(float dirSpread) {
        _dirSpread = dirSpread; return *this; }
    const float &getDirSpread() const { return _dirSpread; }

    SprayEmitter &setRadius(float rad) { _radMean = rad; return *this; }
    const float &getRadius() const { return _radMean; }

    SprayEmitter &setRadiusSpread(float radSpread) {
        _radSpread = radSpread; return *this; }
    const float &getRadSpread() const { return _radSpread; }

    SprayEmitter &setSpeed(float speed) { _speedMean = speed; return *this; }
    const float &getSpeed() const { return _speedMean; }

    SprayEmitter &setSpeedSpread(float speedSpread) {
        _speedSpread = speedSpread; return *this; }
    const float &getSpeedSpread() const { return _speedSpread; }

protected:
    float _rate;
    Vec3 _position;
    Vec3 _dirMean;
    float _dirSpread;
    float _radMean;
    float _radSpread;
    float _speedMean;
    float _speedSpread;

    void _emitParticles(double dt,
        const ParticleSystem *particleSystem,
        std::vector<Particle *> &particles);
};

#endif // __EMITTER_H__
