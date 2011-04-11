#ifndef _SCENERENDER_H_
#define _SCENERENDER_H_

#include <string>
#include <map>

#include "ptr.h"
#include "Graph.h"
#include "Visitor.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Prim.h"
#include "RenderState.h"
#include "RenderPass.h"
#include "Mat.h"

DEF_SCOPED_PTR(SceneRenderer);

class SceneRenderer : public Visitor
{
public:
    virtual void render(const GraphPtr &scene);

    virtual void visitCamera(const CameraConstPtr &camera);
    virtual void visitLight(const LightConstPtr &light);
    virtual void visitTransform(const TransformConstPtr &transform);
    virtual void visitPrim(const PrimConstPtr &prim);

    void setRenderPass(const RenderPassPtr &renderPass) {
        _renderPass = renderPass; }
    void setCameraName(const std::string &cameraName) {
        _cameraName = cameraName; }

    static SceneRendererPtr create(const RenderPassConstPtr &renderPass,
                                   const std::string &cameraName = "") {

        return SceneRendererPtr(new SceneRenderer(renderPass, cameraName));
    }

protected:
    RenderPassConstPtr _renderPass;
    std::string _cameraName;

    RenderState state;

    SceneRenderer(const RenderPassConstPtr &renderPass,
                  const std::string &cameraName = "");
};

#endif // _SCENERENDERVISITOR_H_
