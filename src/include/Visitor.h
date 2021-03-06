#ifndef VISITOR_H_
#define VISITOR_H_

#include <vector>
#include <stack>

#include "Mat.h"

class Node;
class Scene;
class Camera;
class Light;
class Transform;
class Prim;

class Visitor {
public:
    Visitor();

    virtual void visitCamera(Camera *camera) = 0;
    virtual void visitLight(Light *light) = 0;
    virtual void visitTransform(Transform *transform) = 0;
    virtual void visitPrim(Prim *prim) = 0;

protected:
    void _visitChildren(Node *node);
    void _visitNodes(const std::vector<Node *> &nodes);
};

#endif // VISITOR_H_
