#ifndef _PRIM_H_
#define _PRIM_H_

#include <string>

#include "ptr.h"
#include "Node.h"
#include "Visitor.h"
#include "RenderState.h"

DEF_SHARED_PTR(Prim);

class Prim : public Node
{
public:
    Prim(const std::string &name) : Node(name) { };

    virtual void render(RenderState &state) const = 0;
    virtual void accept(VisitorPtr &visitor) const {
        visitor->visitPrim(PrimConstPtr(this)); }
};

#endif // _PRIM_H_
