#ifndef _PRIM_H_
#define _PRIM_H_

#include "Node.h"
#include "Visitor.h"
#include "RenderState.h"

struct Prim : public Node
{
    Prim(const char *name) : Node(name) {};

    virtual void render(RenderState &state) = 0;
    virtual void accept(Visitor *visitor) { visitor->visitPrim(this); }
};

#endif // PRIM_H_
