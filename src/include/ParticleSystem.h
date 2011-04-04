#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <vector>

#include <boost/pool/object_pool.hpp>

#include "Vec.h"
#include "Prim.h"
#include "Particle.h"

#include "Emitter.h"
#include "Field.h"

class ParticleSystem
{
public:
    enum WallType { NONE, BOUNCE, WRAP, KILL };

    ParticleSystem(const Vec3 &size, WallType wallType = WRAP);

    virtual ~ParticleSystem();

    virtual void update(double dt);

    void reset() { _destroy(); }

    void addEmitter(Emitter *emitter) { _emitters.push_back(emitter); }
    void addField(Field *field) { _fields.push_back(field); }

    Particle *newParticle() { return _particlePool.construct(); }

    const Vec3 &getSize() const { return _size; }
    const std::vector<Particle *> &getParticles() { return _particles; }

protected:
    friend class ParticleSystemPrim;

    Vec3 _size;

    boost::object_pool<Particle> _particlePool;
    std::vector<Particle *> _particles;

    std::vector<Emitter *> _emitters;
    std::vector<Field *> _fields;

    WallType _wallType;
    bool _needsDepthSort;

    void _destroy();
};

#endif /* PARTICLESYSTEM_H_ */
