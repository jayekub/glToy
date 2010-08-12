#include <stdio.h>
#include <string.h>

#include "RenderPass.h"

#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Prim.h"
#include "Mat.h"

#include "SceneRenderVisitor.h"

SceneRenderVisitor::SceneRenderVisitor(
        RenderPass *renderPass, const std::string &cameraName) :
    _renderPass(renderPass), _cameraName(cameraName), _currentScene(NULL)
{
}

SceneRenderVisitor::~SceneRenderVisitor()
{
}

void SceneRenderVisitor::render(Scene *scene)
{

    _renderPass->begin();

    glClearColor(0., 0., 0., 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);

    scene->accept(this);

    _renderPass->end();

    _currentScene = NULL;
}

void SceneRenderVisitor::visitScene(Scene *scene)
{
    _currentScene = scene;

    _currentScene->state.reset();
    _currentScene->state.renderPass = _renderPass;

    _visitNodes(scene->globals);

    if (!_currentScene->state.camera) {
        fprintf(stderr, "Couldn't find camera %s. Aborting.\n",
                _cameraName.c_str());
        return;
    }

    _visitChildren(scene);
}

void SceneRenderVisitor::visitCamera(Camera *camera)
{
    // XXX find a faster way to do this!
    if (strcmp(_cameraName.c_str(), camera->name) == 0) {
        int vpWidth = _renderPass->getWidth();
        int vpHeight = _renderPass->getHeight();

        // XXX switch to Mat4 methods rather than gl builtins
        if (camera->projection == Camera::FLAT) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glViewport(0.0, 0.0, vpWidth, vpHeight);
            glOrtho(0, vpWidth, vpHeight, 0, 0, 1);
        } else { // camera->projection == Camera::PERSP
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glViewport(0.0, 0.0, vpWidth, vpHeight);
            gluPerspective(camera->fov, (float) vpWidth / (float) vpHeight,
                           camera->nearClip, camera->farClip);


            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            gluLookAt(camera->position.x, camera->position.y, camera->position.z,
                      camera->center.x, camera->center.y, camera->center.z,
                      camera->up.x, camera->up.y, camera->up.z);
        }

        // build shadow matrix and update light
        if (camera->isShadowCamera) {
            // XXX find some way to cache this?

            // move from [-1, 1] to [0, 1]
            const Mat4x4 bias(0.5, 0.0, 0.0, 0.5,
                              0.0, 0.5, 0.0, 0.5,
                              0.0, 0.0, 0.5, 0.5,
                              0.0, 0.0, 0.0, 1.0);

            Mat4x4 modelView, projection;

            glGetFloatv(GL_MODELVIEW_MATRIX, modelView.v);
            glGetFloatv(GL_PROJECTION_MATRIX, projection.v);

            camera->light->shadowMatrix = bias * projection * modelView;
        }

        _currentScene->state.camera = camera;
    }

    _visitChildren(camera);
}

void SceneRenderVisitor::visitLight(Light *light)
{
    _currentScene->state.lights[light->name] = light;
    _currentScene->state.lightPositions[light->name] =
            _getCurrentTransform().getTranslation();
}

void SceneRenderVisitor::visitTransform(Transform *transform)
{
    //glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    _pushTransform();

    _multTransform(Mat4::translate(transform->translation));
    _multTransform(Mat4::rotate(transform->rotationAngle,
                                transform->rotationAxis));
    _multTransform(Mat4::scale(transform->scale));

    /*
    // XXX cache transform node matrix!!
    glTranslatef(transform->translation.x,
                 transform->translation.y,
                 transform->translation.z);

    glRotatef(transform->rotationAngle,
              transform->rotationAxis.x,
              transform->rotationAxis.y,
              transform->rotationAxis.z);

    glScalef(transform->scale.x,
             transform->scale.y,
             transform->scale.z);
    */

    _visitChildren(transform);

    //glPopMatrix();
    _popTransform();
}

void SceneRenderVisitor::visitPrim(Prim *prim)
{
    prim->render(&(_currentScene->state));

    _visitChildren(prim);
}
