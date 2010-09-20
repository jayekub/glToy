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

    _renderPass->begin();

    glClearColor(0., 0., 0., 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        switch (camera->projection) {
            case Camera::FLAT:
                state.projectionMat = Mat4::ortho(0, vpWidth, vpHeight,
                                                  0, 0, 1);
                break;
            case Camera::ORTHO: {
                int w_2 = camera->width / 2;
                int h_2 = camera->height / 2;

                state.projectionMat =
                    Mat4::ortho(-w_2 - camera->position.x,
                                w_2 - camera->position.x,
                                -h_2 - camera->position.y,
                                h_2 - camera->position.y,
                                camera->nearClip + camera->position.z,
                                camera->farClip + camera->position.z);
                break;
            }
            case Camera::PERSP:
                state.projectionMat =
                    Mat4::perspective(camera->fov,
                                  (float) vpWidth / (float) vpHeight,
                                  camera->nearClip, camera->farClip);

                state.viewMat =
                    Mat4::lookat(camera->position, camera->center, camera->up);
                break;
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

    state.multTransformMat(transform->matrix);

    _visitChildren(transform);

    state.popTransformMat();
}

void SceneRenderVisitor::visitPrim(Prim *prim)
{
    prim->render(state);

    _visitChildren(prim);
}
