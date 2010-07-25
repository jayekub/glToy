#ifndef SCENERENDERVISITOR_H_
#define SCENERENDERVISITOR_H_

#include <string>
#include <vector>

#include "Visitor.h"

class RenderPass;

class Scene;
class Camera;
class Transform;
class Prim;

class SceneRenderVisitor : public Visitor
{
public:
    SceneRenderVisitor(RenderPass *renderPass = NULL,
                       const std::string &cameraName = "");
    virtual ~SceneRenderVisitor();

    void render(Scene *scene);

    void visitScene(Scene *scene);
    void visitCamera(Camera *camera);
    void visitTransform(Transform *transform);
    void visitPrim(Prim *prim);

    void setRenderPass(RenderPass *renderPass) { _renderPass = renderPass; }
    void setCameraName(const std::string &cameraName) {
        _cameraName = cameraName; }

private:
    RenderPass *_renderPass;
    std::string _cameraName;
};

#endif /* SCENERENDERVISITOR_H_ */
