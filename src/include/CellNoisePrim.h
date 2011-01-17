#ifndef _CELLNOISEPRIM_H_
#define _CELLNOISEPRIM_H_

#include "utils.h"
#include "Program.h"
#include "RenderState.h"
#include "ParticleSystemPrim.h"

class CellNoisePrim : public ParticleSystemPrim
{
public:
    // particleSize is radius of particles as a fraction of screen width
    CellNoisePrim(const char *name, ParticleSystem *particleSystem,
                   float particleSize);

private:
    float _particleSize;

    Program _cellNoiseProgram;

    void _preRender(RenderState &state);
    void _postRender(RenderState &state);
};

#endif /* _CELLNOISEPRIM_H_ */
