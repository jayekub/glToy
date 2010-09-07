#include <stdio.h>
#include <string.h>

#include "RenderPass.h"

#include "Graph.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Prim.h"
#include "Mat.h"

#include "SceneRenderVisitor.h"

SceneRenderVisitor::SceneRenderVisitor(
        RenderPass *renderPass, const std::string &cameraName) :
    _renderPass(renderPass), _cameraName(cameraName)
{
}

SceneRenderVisitor::~SceneRenderVisitor()
{
}

void SceneRenderVisitor::render(Graph *scene)
{
    state.reset();

    state.renderPass = _renderPass;
    state.cameraName = _cameraName;

//    fprintf(stderr, "initial transform is %s\n",
//            state.getTransformMat().toString().c_str());

    _renderPass->begin();

    glClearColor(0., 0., 0., 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);

    _visitNodes(scene->globals);

    if (!state.camera) {
        fprintf(stderr, "Couldn't find camera %s. Aborting.\n",
                state.cameraName.c_str());
        return;
    }

    scene->root->accept(this);

    _renderPass->end();
}

void SceneRenderVisitor::visitCamera(Camera *camera)
{
    // XXX find a faster way to do this!
    if (strcmp(state.cameraName.c_str(), camera->name) == 0) {
        int vpWidth = state.renderPass->getWidth();
        int vpHeight = state.renderPass->getHeight();

        glViewport(0.0, 0.0, vpWidth, vpHeight);

        if (camera->projection == Camera::FLAT) {
            state.projectionMat = Mat4::ortho(0, vpWidth, vpHeight, 0, 0, 1);
        } else { // camera->projection == Camera::PERSP
            state.projectionMat =
                Mat4::perspective(camera->fov,
                                  (float) vpWidth / (float) vpHeight,
                                  camera->nearClip, camera->farClip);

            state.viewMat =
                Mat4::lookat(camera->position, camera->center, camera->up);
        }

        // build shadow matrix and update light
        if (camera->isShadowCamera) {
            // XXX find some way to cache this?

            // move from [-1, 1] to [0, 1]
            const Mat4x4 bias(0.5, 0.0, 0.0, 0.5,
                              0.0, 0.5, 0.0, 0.5,
                              0.0, 0.0, 0.5, 0.5,
                              0.0, 0.0, 0.0, 1.0);

            camera->light->shadowMatrix =
                bias * state.projectionMat * state.viewMat;
        }

        state.camera = camera;
    }

    _visitChildren(camera);
}

void SceneRenderVisitor::visitLight(Light *light)
{
    state.lights[light->name] = light;
    state.lightPositions[light->name] =
        state.getTransformMat().getTranslation();
}

void SceneRenderVisitor::visitTransform(Transform *transform)
{
    state.pushTransformMat();

    state.multTransformMat(Mat4::translate(transform->translation));
    state.multTransformMat(Mat4::rotate(transform->rotationAngle,
                                        transform->rotationAxis));
    state.multTransformMat(Mat4::scale(transform->scale));

//    fprintf(stderr, "applied %s, transform is %s\n", transform->name,
//            state.getTransformMat().toString().c_str());

    _visitChildren(transform);

    state.popTransformMat();
}

void SceneRenderVisitor::visitPrim(Prim *prim)
{
    prim->render(state);

    _visitChildren(prim);
}
