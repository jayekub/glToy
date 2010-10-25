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

    void emitOnce(int numParticles, float maxSpeed, float meanRadius = 0.);

private:
    int _numToEmit;
    float _maxSpeed;
    float _meanRadius;

    void _emitParticles(double dt, const ParticleSystem *particleSystem,
                        std::vector<Particle *> &particles);
};

class SprayEmitter : public Emitter
{
public:
    SprayEmitter(float rate, Vec3 position,
                 Vec3 dirMean, float dirSpread,
                 float radMean, float radSpread,
                 float speedMean, float speedSpread);

    //void update(double dt) { }

protected:
    float _rate;
    Vec3 _position;
    Vec3 _dirMean;
    float _dirSpread;
    float _radMean;
    float _radSpread;
    float _speedMean;
    float _speedSpread;

    // basis vectors that lie in the plane of the direction cone base
    Vec3 _A, _B;

    void _emitParticles(double dt,
        const ParticleSystem *particleSystem,
        std::vector<Particle *> &particles);
};

#endif // __EMITTER_H__
