#include <stdio.h>
#include <string.h>

#include "RenderPass.h"

#include "Scene.h"
#include "Camera.h"
#include "Transform.h"
#include "Prim.h"

#include "SceneRenderVisitor.h"

SceneRenderVisitor::SceneRenderVisitor(
        RenderPass *renderPass, const std::string &cameraName) :
    _renderPass(renderPass), _cameraName(cameraName)
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
}

void SceneRenderVisitor::visitScene(Scene *scene)
{
    _visitChildren(scene);
}

void SceneRenderVisitor::visitCamera(Camera *camera)
{
    if (strcmp(_cameraName.c_str(), camera->name) == 0) {
        if (camera->projection == Camera::FLAT) {
            _renderPass->setFlatProjection();
        } else {
            _renderPass->setPerspProjection(
                camera->fov, camera->nearClip, camera->farClip);
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(camera->position.x, camera->position.y, camera->position.z,
                  camera->center.x, camera->center.y, camera->center.z,
                  camera->up.x, camera->up.y, camera->up.z);
    }

    _visitChildren(camera);
}

void SceneRenderVisitor::visitTransform(Transform *transform)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

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

    _visitChildren(transform);

    glPopMatrix();
}

void SceneRenderVisitor::visitPrim(Prim *prim)
{
    prim->render();

    _visitChildren(prim);
}
