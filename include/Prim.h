#ifndef PRIM_H_
#define PRIM_H_

#include "Node.h"
#include "Visitor.h"
#include "Scene.h"

struct Prim : public Node
{
    virtual void render(const Scene::State *state) = 0;
    virtual void accept(Visitor *visitor) { visitor->visitPrim(this); }

    Prim(const char *name) : Node(name) {};
};

#endif /* PRIM_H_ */
