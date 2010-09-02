#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <vector>

#include <boost/foreach.hpp>

#include "Vec.h"
#include "Particle.h"
#include "Prim.h"

template<class V>
class ParticleSystem : public Prim
{
public:
    ParticleSystem(const char *name, const V size) : Prim(name), _size(size) {
        glGenBuffers(1, &_vertexBuffer);
        _vertexBufferSize = 0;
    }

    virtual ~ParticleSystem() {
        _destroy();
        glDeleteBuffers(1, &_vertexBuffer);
    }

    // XXX refactor to Emitter
    void emitRandom(int num, double maxVelocity) {
        for (int i = 0; i < num; ++i) {
            Particle<V> *p = new Particle<V>();

            p->position = V::randVec(0., 1.);
            p->velocity = maxVelocity * V::randVec(-1, 1).normalize();

            _particles.push_back(p);
        }
    }

    virtual void update(double dt) {
        BOOST_FOREACH(Particle<V> *p, _particles) {
            //fprintf(stderr, "pos %s vel %s\n",
            //        p->position.toString().c_str(),
            //        p->velocity.toString().c_str());
            p->position += p->velocity * dt;
            _wrapParticle(p);
        }
    }

    virtual void render(const RenderState &state) {
        // put current particle positions in vertex buffer and allocate more
        // space if necessary
        const size_t numParticles = _particles.size();

        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

        if (_vertexBufferSize < numParticles) {
            glBufferData(GL_ARRAY_BUFFER, numParticles * sizeof(V), 0,
                         GL_DYNAMIC_DRAW);
            _vertexBufferSize = numParticles;
        }

        V *vertices = (V *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

        for (size_t p = 0; p < numParticles; ++p) {
            //fprintf(stderr, "%d: position = %s\n",
            //        p, _particles[p]->position.toString().c_str());
            vertices[p] = _particles[p]->position;
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnableClientState(GL_VERTEX_ARRAY);

        // allow subclasses to setup a Program for rendering
        _prepRender(state);

        // send particle positions to the card
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

        const int numCoords = sizeof(V) / sizeof(vec_t);

        glVertexPointer(numCoords, GL_FLOAT, 0, 0);
        glDrawArrays(GL_POINTS, 0, numParticles);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void reset() { _destroy(); }

    std::vector<Particle<V> *> &getParticles() { return _particles; }

protected:
    V _size;
    std::vector<Particle<V> *> _particles;

    size_t _vertexBufferSize;
    GLuint _vertexBuffer;

    void _destroy() {
        BOOST_FOREACH(Particle<V> *p, _particles) {
            delete p;
        }
        _particles.clear();
    }

    virtual void _prepRender(const RenderState &state) {
        // subclasses can setup Program etc here
    }

    void _wrapParticle(Particle<V> *p) const;
};

#define WRAP_DIM(p, d) \
        if (p->position.d < 0.0) \
            p->position.d += 1.0; \
        else if (p->position.d > 1.0) \
            p->position.d -= 1.0;

template<>
inline void ParticleSystem<Vec2>::_wrapParticle(Particle<Vec2> *p) const
{
    WRAP_DIM(p, x);
    WRAP_DIM(p, y);
}

template<>
inline void ParticleSystem<Vec3>::_wrapParticle(Particle<Vec3> *p) const
{
    WRAP_DIM(p, x);
    WRAP_DIM(p, y);
    WRAP_DIM(p, z);
}

#undef WRAP_DIM

/*

*/

#endif /* PARTICLESYSTEM_H_ */
