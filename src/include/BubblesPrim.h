#ifndef _BUBBLESPRIM_H_
#define _BUBBLESPRIM_H_

#include "RenderState.h"
#include "Program.h"
#include "ParticleSystemPrim.h"

class ParticleSystem;

class BubblesPrim : public ParticleSystemPrim
{
public:
    BubblesPrim(const char *name, ParticleSystem *particleSystem);

private:
    Program _bubblesProgram;

    void _preRender(RenderState &state);
    void _postRender(RenderState &state);
};

#endif /* _BUBBLESPRIM_H_ */
