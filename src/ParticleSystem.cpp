#include <string.h>

#include <boost/foreach.hpp>

#include "Particle.h"
#include "Camera.h"

#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(
    const char *name,
    const Vec3 &size,
    WallType wallType,
    bool needsDepthSort) :

    Prim(name), _size(size), _wallType(wallType),
    _needsDepthSort(needsDepthSort)
{
    glGenBuffers(1, &_particleBuffer);
    glGenTextures(1, &_particleTexture);

    _particleBufferSize = 0;
}

ParticleSystem::~ParticleSystem()
{
    _destroy();

    glDeleteBuffers(1, &_particleBuffer);
    glDeleteTextures(1, &_particleTexture);
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

    BOOST_FOREACH(Particle *p, _particles) {
        p->position += p->velocity * dt;

        BOOST_FOREACH(Field *f, _fields) {
            // return value true means this particle should be killed
            if (f->_affectParticle(p, dt, this)) {
                //_particles.erase(p);
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
        }
    }

#undef WRAP_DIM
#undef BOUNCE_DIM
}

void ParticleSystem::render(RenderState &state)
{
    // if necessary, sort particles by distances from camera so that further
    // away particles are rendered first
    if (_needsDepthSort) {
        Mat4 invModelMat = state.getTransformMat().inverse();
        Vec3 localCameraPos = invModelMat.ptransform(
            state.camera->position);

        //std::sort(_particles.begin(), _particles.end(),
        //          _ParticleLt(localCameraPos));
        std::stable_sort(_particles.begin(), _particles.end(),
                         _ParticleLt(localCameraPos, this));
    }

    // put current particle positions in vertex buffer and allocate more
    // space if necessary
    const size_t numParticles = _particles.size();

    glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer);

    if (_particleBufferSize < numParticles) {
        glBufferData(GL_ARRAY_BUFFER, numParticles * sizeof(Particle), 0,
                     GL_DYNAMIC_DRAW);
        _particleBufferSize = numParticles;
    }

    Particle *particles = (Particle *) glMapBuffer(
            GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    // _particles may be sorted in ascending order of distance from camera,
    // so reverse the order for rendering to make the furthest particles
    // render first
    for (size_t p = 0; p < numParticles; ++p)
        particles[p] = *(_particles[numParticles - p - 1]);

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // hook up vertex buffer to vertex texture
    /*
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER, _particleTexture);

    glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, _particleBuffer);

    glBindTexture(GL_TEXTURE_BUFFER, 0);
    */

    // get ready to render

    // allow subclasses to setup a Program and other state for rendering
    _preRender(state);

    // send particle positions to the card
    glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer);
    glDrawArrays(GL_POINTS, 0, numParticles);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // allow subclasses to possibly render more stuff and also cleanup
    // after themselves
    _postRender(state);
}

void ParticleSystem::_destroy() {
    BOOST_FOREACH(Particle *p, _particles)
        delete p;

    BOOST_FOREACH(Emitter *e, _emitters)
        delete e;

    BOOST_FOREACH(Field *f, _fields)
        delete f;

    _particles.clear();
    _emitters.clear();
    _fields.clear();
}

bool ParticleSystem::_particlelt(const Particle *a, const Particle *b,
                                 const Vec3 &cameraPos) const {
    vec_t aDist = (a->position - cameraPos).length();
    vec_t bDist = (b->position - cameraPos).length();

    return aDist < bDist;
}

