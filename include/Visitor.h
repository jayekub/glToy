#ifndef VISITOR_H_
#define VISITOR_H_

class Node;
class Scene;
class Camera;
class Transform;
class Prim;

class Visitor {
public:
    virtual void visitScene(Scene *scene) = 0;
    virtual void visitCamera(Camera *camera) = 0;
    virtual void visitTransform(Transform *transform) = 0;
    virtual void visitPrim(Prim *prim) = 0;

protected:
    void _visitChildren(Node *node);
};

#endif // VISITOR_H_
