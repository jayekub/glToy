#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <vector>

#include "Vec.h"
#include "Prim.h"
#include "Particle.h"

#include "Emitter.h"
#include "Field.h"

// TODO split into ParticleSystem and ParticleSystemPrim or
//      ParticleSystemRenderer

class ParticleSystem : public Prim
{
public:
    enum WallType { NONE, BOUNCE, WRAP, KILL };

    ParticleSystem(const char *name, const Vec3 &size,
                   WallType wallType = WRAP,
                   bool needsDepthSort = false);

    virtual ~ParticleSystem();

    virtual void update(double dt);
    virtual void render(RenderState &state);

    void reset() { _destroy(); }

    void addEmitter(Emitter *emitter) { _emitters.push_back(emitter); }
    void addField(Field *field) { _fields.push_back(field); }

    const Vec3 &getSize() const { return _size; }
    const std::vector<Particle *> &getParticles() { return _particles; }

protected:
    struct _ParticleLt {
        _ParticleLt(const _ParticleLt *impl) : _impl(impl) {}

        virtual bool operator()(const Particle *a, const Particle *b) const {
            return (*_impl)(a, b);
        }

    private:
        const _ParticleLt *_impl;
    };

    Vec3 _size;
    std::vector<Particle *> _particles;
    std::vector<Emitter *> _emitters;
    std::vector<Field *> _fields;

    WallType _wallType;
    bool _needsDepthSort;

    size_t _particleBufferSize;
    GLuint _particleBuffer, _particleTexture;

    void _destroy();

    virtual _ParticleLt *_getParticleLtImpl(const RenderState &state) const;

    // subclasses can setup Program etc here
    virtual void _preRender(RenderState &state) {}

    // subclasses can render additional stuff and cleanup here
    virtual void _postRender(RenderState &state) {}
};

#endif /* PARTICLESYSTEM_H_ */
