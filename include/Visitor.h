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

    virtual void visitScene(Scene *scene) = 0;
    virtual void visitCamera(Camera *camera) = 0;
    virtual void visitLight(Light *light) = 0;
    virtual void visitTransform(Transform *transform) = 0;
    virtual void visitPrim(Prim *prim) = 0;

protected:
    std::stack<Mat4> _transformStack;
    //Mat4 _currentTransform;

    void _visitChildren(Node *node);
    void _visitNodes(const std::vector<Node *> &nodes);

    void _pushTransform();
    void _popTransform();
    void _multTransform(const Mat4 &mat);

    Mat4 _getCurrentTransform() const { return _transformStack.top(); }
};

#endif // VISITOR_H_
