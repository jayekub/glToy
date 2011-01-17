#include "ParticleSystem.h"
#include "Camera.h"

#include "ParticleSystemPrim.h"

ParticleSystemPrim::ParticleSystemPrim(
    const char *name,
    ParticleSystem *particleSystem,
    bool needsDepthSort) :

    Prim(name), _particleSystem(particleSystem),
    _needsDepthSort(needsDepthSort)
{
    glGenBuffers(1, &_particleBuffer);
    glGenTextures(1, &_particleTexture);

    _particleBufferSize = 0;
}

ParticleSystemPrim::~ParticleSystemPrim()
{
    glDeleteBuffers(1, &_particleBuffer);
    glDeleteTextures(1, &_particleTexture);
}

void ParticleSystemPrim::render(RenderState &state)
{
    //std::vector<Particle *> &particles = _particleSystem->_particles;

    // if necessary, sort particles by distances from camera so that further
    // away particles are rendered first
    if (_needsDepthSort) {
        std::stable_sort(_particleSystem->_particles.begin(),
                         _particleSystem->_particles.end(),
                         _ParticleLt(_getParticleLtImpl(state)));
    }

    // put current particle positions in vertex buffer and allocate more
    // space if necessary
    const size_t numParticles = _particleSystem->_particles.size();

    glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer);

    if (_particleBufferSize < numParticles) {
        glBufferData(GL_ARRAY_BUFFER, numParticles * sizeof(Particle), 0,
                     GL_DYNAMIC_DRAW);
        _particleBufferSize = numParticles;
    }

    Particle *particlesBuf = (Particle *) glMapBuffer(
            GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    // particles may be sorted in ascending order of distance from camera,
    // so reverse the order for rendering to make the furthest particles
    // render first
    for (size_t p = 0; p < numParticles; ++p)
        particlesBuf[p] = *(_particleSystem->_particles[numParticles - p - 1]);

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

// sort according to viewpoint distance
ParticleSystemPrim::_ParticleLt *
ParticleSystemPrim::_getParticleLtImpl(const RenderState &state) const
{
    struct _ParticleLtImpl : public _ParticleLt {

        _ParticleLtImpl(const RenderState &state,
                        const ParticleSystem *particleSystem) :
            _ParticleLt(NULL), _particleSystem(particleSystem) {

            Mat4 invModelMat = state.getTransformMat().inverse();
            _localCameraPos = invModelMat.ptransform(state.camera->position);
        }

        bool operator()(const Particle *a, const Particle *b) const {
            vec_t aDist = (a->position - _localCameraPos).length();
            vec_t bDist = (b->position - _localCameraPos).length();

            return aDist < bDist;
        }

    private:
        Vec3 _localCameraPos;
        const ParticleSystem *_particleSystem;
    };

    static _ParticleLtImpl *particleLtImpl = NULL;

    delete particleLtImpl;
    return new _ParticleLtImpl(state, _particleSystem);
}
