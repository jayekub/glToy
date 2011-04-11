#ifndef _CELLNOISEPRIM_H_
#define _CELLNOISEPRIM_H_

#include <string>

#include "ptr.h"
#include "utils.h"
#include "Program.h"
#include "RenderState.h"
#include "ParticleSystemPrim.h"

DEF_SHARED_PTR(CellNoisePrim)

class CellNoisePrim : public ParticleSystemPrim
{
public:
    ParticleSystemPtr particleSystem;

    // particleSize is radius of particles as a fraction of screen width
    float particleSize;

    DEF_NODE_CREATE(CellNoisePrim);

private:
    Program _cellNoiseProgram;

    void _preRender(RenderState &state);
    void _postRender(RenderState &state);

    CellNoisePrim(const std::string &name);
};

#endif /* _CELLNOISEPRIM_H_ */
