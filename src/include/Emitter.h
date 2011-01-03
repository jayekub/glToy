#ifndef __EMITTER_H__
#define __EMITTER_H__

#include <vector>

class Particle;
class ParticleSystem;

class Emitter
{
public:
    virtual void update(double dt) {};

protected:
    friend class ParticleSystem;

    virtual void _emitParticles(double dt,
        const ParticleSystem *particleSystem,
        std::vector<Particle *> &particles) = 0;
};

class RandomEmitter : public Emitter
{
public:
    RandomEmitter();

    void emitOnce(int numParticles, float meanSpeed, float meanRadius = 0.);

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

    SprayEmitter &setRate(float rate) {_rate = rate; return *this;}

    SprayEmitter &setPosition(const Vec3 &position) {
        _position = position; return *this;}

    SprayEmitter &setDir(const Vec3 &dir) {_dirMean = dir; return *this;}
    SprayEmitter &setDirSpread(float dirSpread) {
        _dirSpread = dirSpread; return *this;}

    SprayEmitter &setRadius(float rad) {_radMean = rad; return *this;}
    SprayEmitter &setRadiusSpread(float radSpread) {
        _radSpread = radSpread; return *this;}

    SprayEmitter &setSpeed(float speed) {_speedMean = speed; return *this;}
    SprayEmitter &setSpeedSpread(float speedSpread) {
        _speedSpread = speedSpread; return *this;}

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
