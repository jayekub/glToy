#ifndef SHADOWRENDERVISITOR_H_
#define SHADOWRENDERVISITOR_H_

#include "SceneRenderVisitor.h"
#include "Mat.h"

// TODO rename *RenderVisitor to *Renderer

class ShadowRenderVisitor : public SceneRenderVisitor
{
public:
    ShadowRenderVisitor(RenderPass *renderPass = NULL,
                        const std::string &cameraName = "") :
        SceneRenderVisitor(renderPass, cameraName) {};
    virtual ~ShadowRenderVisitor() {};

    virtual void visitCamera(Camera *camera);

    Mat4x4 getShadowMatrix() const { return _shadowMatrix; }

protected:
    Mat4x4 _shadowMatrix;
};

#endif /* SHADOWRENDERVISITOR_H_ */
