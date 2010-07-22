#include <stdio.h>

#include <boost/foreach.hpp>

#include "ParticleSystem.h"
#include "utils.h"

ParticleSystem::ParticleSystem(int num, double maxVelocity, double friction) :
    _num(num), _maxVelocity(maxVelocity), _friction(friction)
{
    addRandom(num, maxVelocity);
}

ParticleSystem::~ParticleSystem()
{
    _destroy();
}

void ParticleSystem::addRandom(int num, double maxVelocity) {
    for (int i = 0; i < num; ++i) {
        Particle *p = new Particle();

        p->position.x = randFloat();
        p->position.y = randFloat();

        p->velocity = Vec2(2.0 * randFloat() - 1.0, 2.0 * randFloat() - 1.0);
        p->velocity = p->velocity.normalize().mult(randFloat() * maxVelocity);

        _particles.push_back(p);
    }
}

void ParticleSystem::update(double dt)
{
    BOOST_FOREACH(Particle *p, _particles) {
        p->position.x += p->velocity.x * dt;
        p->position.y += p->velocity.y * dt;

        p->velocity.x *= 1.0 - _friction;
        p->velocity.y *= 1.0 - _friction;

        WRAP_PARTICLE(p);
    }
}

void ParticleSystem::reset()
{
    _destroy();
    addRandom(_num, _maxVelocity);
}


void ParticleSystem::_destroy()
{
    BOOST_FOREACH(Particle *p, _particles) {
        delete p;
    }
    _particles.clear();
}
