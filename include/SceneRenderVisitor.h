#ifndef SCENERENDERVISITOR_H_
#define SCENERENDERVISITOR_H_

#include <string>
#include <map>

#include "Visitor.h"
#include "Mat.h"

class RenderPass;

class SceneRenderVisitor : public Visitor
{
public:
    SceneRenderVisitor(RenderPass *renderPass = NULL,
                       const std::string &cameraName = "");
    virtual ~SceneRenderVisitor();

    virtual void render(Scene *scene);

    virtual void visitScene(Scene *scene);
    virtual void visitCamera(Camera *camera);
    virtual void visitLight(Light *light);
    virtual void visitTransform(Transform *transform);
    virtual void visitPrim(Prim *prim);

    void setRenderPass(RenderPass *renderPass) { _renderPass = renderPass; }
    void setCameraName(const std::string &cameraName) {
        _cameraName = cameraName; }


protected:
    RenderPass *_renderPass;
    std::string _cameraName;

    Scene *_currentScene;

    virtual Mat4x4 _getCurrentTransform() const;
};

#endif /* SCENERENDERVISITOR_H_ */
