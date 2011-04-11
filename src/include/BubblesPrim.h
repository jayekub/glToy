#ifndef _BUBBLESPRIM_H_
#define _BUBBLESPRIM_H_

#include <string>

#include "ptr.h"
#include "RenderState.h"
#include "Program.h"
#include "ParticleSystem.h"
#include "Node.h"
#include "ParticleSystemPrim.h"

DEF_SHARED_PTR(BubblesPrim);

class BubblesPrim : public ParticleSystemPrim
{
public:
    ParticleSystemPtr particleSystem;

    DEF_NODE_CREATE(BubblesPrim);

private:
    Program _bubblesProgram;

    BubblesPrim(const std::string &name_);

    void _preRender(RenderState &state);
    void _postRender(RenderState &state);
};

#endif /* _BUBBLESPRIM_H_ */
