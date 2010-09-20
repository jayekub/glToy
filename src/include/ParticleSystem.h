#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <string.h>
#include <vector>

#include <boost/foreach.hpp>

#include "Vec.h"
#include "Particle.h"
#include "Prim.h"
#include "Camera.h"

template<class P = Particle<> >
class ParticleSystem : public Prim
{
public:
    enum WallType { NONE, BOUNCE, WRAP };

    typedef P particle_t;

    ParticleSystem(const char *name, const Vec3 &size,
                   WallType wallType = WRAP,
                   bool needsDepthSort = false) :
        Prim(name), _size(size), _wallType(wallType),
        _needsDepthSort(needsDepthSort) {

        glGenBuffers(1, &_particleBuffer);
        glGenTextures(1, &_particleTexture);

        _particleBufferSize = 0;

    }

    virtual ~ParticleSystem() {
        _destroy();

        glDeleteBuffers(1, &_particleBuffer);
        glDeleteTextures(1, &_particleTexture);
    }

    // XXX refactor to Emitter
    void emitRandom(int num, double maxVelocity) {
        for (int i = 0; i < num; ++i) {
            particle_t *p = new particle_t();

            p->position = Vec3::randVec(0., 1.);
            p->velocity = maxVelocity * Vec3::randVec(-1, 1).normalize();

            _setRandomAttributes(p);

            _particles.push_back(p);
        }
    }

    virtual void update(double dt) {

#define WRAP_DIM(p, d) \
    p->position.d += p->position.d < 0. ? \
         1. : p->position.d > 1. ? -1. : 0.;

#define BOUNCE_DIM(p, d) \
    p->velocity.d *= p->position.d < 0. || p->position.d > 1. ? -1. : 1.; \
    p->position.d = p->position.d < 0. ? \
        0. : p->position.d > 1. ? 1. : p->position.d; \

        BOOST_FOREACH(particle_t *p, _particles) {
            //fprintf(stderr, "pos %s vel %s\n",
            //        p->position.toString().c_str(),
            //        p->velocity.toString().c_str());
            p->position += p->velocity * dt;

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

    virtual void render(RenderState &state) {
        state.pushTransformMat();
        state.multTransformMat(Mat4::scale(_size));

        // if necessary, sort particles by distances from camera so that further
        // away particles are rendered first
        if (_needsDepthSort) {
            Mat4 invModelMat = state.getTransformMat().inverse();
            Vec3 localCameraPos = invModelMat.ptransform(
                state.camera->position);

            //std::sort(_particles.begin(), _particles.end(),
            //          _ParticleLt(localCameraPos));
            std::stable_sort(_particles.begin(), _particles.end(),
                             _ParticleLt(localCameraPos));
        }

        // put current particle positions in vertex buffer and allocate more
        // space if necessary
        const size_t numParticles = _particles.size();

        glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer);

        if (_particleBufferSize < numParticles) {
            glBufferData(GL_ARRAY_BUFFER, numParticles * sizeof(particle_t), 0,
                         GL_DYNAMIC_DRAW);
            _particleBufferSize = numParticles;
        }

        particle_t *particles = (particle_t *) glMapBuffer(
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

        state.popTransformMat();
    }

    void reset() { _destroy(); }

    std::vector<particle_t *> &getParticles() { return _particles; }

protected:
    struct _ParticleLt
    {
        Vec3 cameraPos;

        _ParticleLt(const Vec3 &cameraPos_) : cameraPos(cameraPos_) {}

        bool operator()(const particle_t *a, const particle_t *b) const {
            vec_t aDist = (a->position - cameraPos).length();
            vec_t bDist = (b->position - cameraPos).length();

            return aDist < bDist;
        }
    };

    Vec3 _size;
    std::vector<particle_t *> _particles;

    size_t _particleBufferSize;
    GLuint _particleBuffer, _particleTexture;

    WallType _wallType;
    bool _needsDepthSort;

    void _destroy() {
        BOOST_FOREACH(particle_t *p, _particles) {
            delete p;
        }
        _particles.clear();
    }

    virtual void _setRandomAttributes(particle_t *p) const {
        // subclasses can particle attributes here
    }

    virtual void _preRender(RenderState &state) {
        // subclasses can setup Program etc here
    }

    virtual void _postRender(RenderState &state) {
        // subclasses can render additional stuff and cleanup here
    }
};

#endif /* PARTICLESYSTEM_H_ */
