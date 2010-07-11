#include <stdio.h>

#include <boost/foreach.hpp>

#include "ParticleSystem.h"
#include "utils.h"

ParticleSystem::ParticleSystem(int size, double maxVelocity) :
    _size(size), _maxVelocity(maxVelocity)
{
    _init();
}

ParticleSystem::~ParticleSystem()
{
    BOOST_FOREACH(Particle *p, _particles) {
        delete p;
    }
}

void ParticleSystem::update(double dt) {
    BOOST_FOREACH(Particle *p, _particles) {
        p->position.x += p->velocity.x * dt;
        p->position.y += p->velocity.y * dt;

        if (p->position.x < 0.0)
            p->position.x += 1.0;
        else if (p->position.x > 1.0)
            p->position.x -= 1.0;

        if (p->position.y < 0.0)
            p->position.y += 1.0;
        else if (p->position.y > 1.0)
            p->position.y -= 1.0;
    }
}

void ParticleSystem::_init() {
    for (int i = 0; i < _size; ++i) {
        Particle *p = new Particle();

        p->position.x = randFloat();
        p->position.y = randFloat();

        p->velocity = Vec2d(2.0 * randFloat() - 1.0, 2.0 * randFloat() - 1.0);
        p->velocity = p->velocity.normalize().mult(_maxVelocity);

        _particles.push_back(p);
    }
}
