#ifndef _PARTICLESYSTEMPRIM_H_
#define _PARTICLESYSTEMPRIM_H_

#include "RenderState.h"
#include "Program.h"

#include "Prim.h"

class Particle;
class ParticleSystem;

class ParticleSystemPrim : public Prim
{
public:
    ParticleSystemPrim(const char *name, ParticleSystem *particleSystem,
                       bool needsDepthSort = false);
    virtual ~ParticleSystemPrim();

    virtual void render(RenderState &state);

protected:
    struct _ParticleLt {
        _ParticleLt(const _ParticleLt *impl) : _impl(impl) {}

        virtual bool operator()(const Particle *a, const Particle *b) const {
            return (*_impl)(a, b);
        }

    private:
        const _ParticleLt *_impl;
    };

    ParticleSystem *_particleSystem;
    bool _needsDepthSort;

    size_t _particleBufferSize;
    GLuint _particleBuffer, _particleTexture;

    virtual _ParticleLt *_getParticleLtImpl(const RenderState &state) const;

    // subclasses can setup Program etc here
    virtual void _preRender(RenderState &state) {}

    // subclasses can render additional stuff and cleanup here
    virtual void _postRender(RenderState &state) {}
};

#endif /* _PARTICLESYSTEMPRIM_H_ */
