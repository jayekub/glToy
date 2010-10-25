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
    enum WallType { NONE, BOUNCE, WRAP };

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
        _ParticleLt(const Vec3 &cameraPos,
                    const ParticleSystem *particleSystem) :
            _cameraPos(cameraPos), _particleSystem(particleSystem) {}

        bool operator()(const Particle *a, const Particle *b) const {
            return _particleSystem->_particlelt(a, b, _cameraPos);
        }

    private:
        const Vec3 _cameraPos;
        const ParticleSystem *_particleSystem;
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

    virtual bool _particlelt(const Particle *a, const Particle *b,
                             const Vec3 &cameraPos) const;

    // subclasses can setup Program etc here
    virtual void _preRender(RenderState &state) {}

    // subclasses can render additional stuff and cleanup here
    virtual void _postRender(RenderState &state) {}
};

#endif /* PARTICLESYSTEM_H_ */
