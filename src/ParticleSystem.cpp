#include <cstring>

#include <boost/foreach.hpp>

#include "Particle.h"

#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const Vec3 &size, WallType wallType) :
    _size(size), _wallType(wallType)
{
}

ParticleSystem::~ParticleSystem()
{
    _destroy();
}

void ParticleSystem::update(double dt)
{
    BOOST_FOREACH(Emitter *e, _emitters) {
        e->_emitParticles(dt, this, _particles);
    }

#define WRAP_DIM(p, d) \
p->position.d += p->position.d < 0. ? \
     _size.d : p->position.d > _size.d ? -_size.d : 0.;

#define BOUNCE_DIM(p, d) \
p->velocity.d *= p->position.d < 0. || p->position.d > _size.d ? -1. : 1.;\
p->position.d = p->position.d < 0. ? \
    0. : p->position.d > _size.d ? _size.d : p->position.d; \

#define KILL_DIM(p, d) \
killParticle |= p->position.d < 0 || p->position.d > _size.d;

    std::vector<Particle *> updatedParticles;

    updatedParticles.reserve(_particles.size());

    BOOST_FOREACH(Particle *p, _particles) {
        bool killParticle = false;

        p->position += p->velocity * dt;

        BOOST_FOREACH(Field *f, _fields) {
            // return value true means this particle should be killed
            if (f->_affectParticle(p, dt, this)) {
                killParticle = true;
            }
        }

        switch(_wallType) {
            case NONE:
                break;

            case BOUNCE:
                BOUNCE_DIM(p, x);
                BOUNCE_DIM(p, y);
                BOUNCE_DIM(p, z);

            case WRAP:
                WRAP_DIM(p, x);
                WRAP_DIM(p, y);
                WRAP_DIM(p, z);
                break;

            case KILL:
                KILL_DIM(p, x);
                KILL_DIM(p, y);
                KILL_DIM(p, z);
                break;
        }

        if (killParticle)
            _particlePool.destroy(p);
        else
            updatedParticles.push_back(p);
    }

    _particles = updatedParticles;

#undef WRAP_DIM
#undef BOUNCE_DIM
#undef KILL_DIM
}

void ParticleSystem::_destroy() {
    BOOST_FOREACH(Particle *p, _particles)
        _particlePool.destroy(p);

    BOOST_FOREACH(Emitter *e, _emitters)
        delete e;

    BOOST_FOREACH(Field *f, _fields)
        delete f;

    _particles.clear();
    _emitters.clear();
    _fields.clear();
}
