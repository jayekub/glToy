#ifndef _SPRAYPRIM_H_
#define _SPRAYPRIM_H_

#include "RenderState.h"
#include "Program.h"
#include "ParticleSystemPrim.h"

// uses point sprite rendering. particles are sorted according to view *plane*
// distance.
class SprayPrim : public ParticleSystemPrim
{
public:
    SprayPrim(const char *name, ParticleSystem *particleSystem);

private:
    float _radius;

    Program _sprayProgram;

    _ParticleLt *_getParticleLtImpl(const RenderState &state) const;
    void _preRender(RenderState &state);
    void _postRender(RenderState &state);
};

#endif /* _SPRAYPRIM_H_ */
