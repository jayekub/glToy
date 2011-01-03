#ifndef SPRAY_H_
#define SPRAY_H_

#include "RenderState.h"
#include "Program.h"
#include "ParticleSystem.h"

// uses point sprite rendering. particles are sorted according to view *plane*
// distance.
class Spray : public ParticleSystem
{
public:
    Spray(const char *name, const Vec3 &size);
    virtual ~Spray();

private:
    float _radius;

    Program _sprayProgram;

    _ParticleLt *_getParticleLtImpl(const RenderState &state) const;
    void _preRender(RenderState &state);
    void _postRender(RenderState &state);
};

#endif /* SPRAY_H_ */
