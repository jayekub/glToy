#include "RenderPass.h"

RenderPass::RenderPass(int width, int height)
{
    setSize(width, height);
}

RenderPass::~RenderPass()
{
}

void RenderPass::setSize(int width, int height)
{
    _width = width;
    _height = height;
}

void RenderPass::setOrthoProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0.0, 0.0, _width, _height);
    glOrtho(0, _width, _height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}

void RenderPass::setPerspProjection(float fov, float near, float far)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0.0, 0.0, _width, _height);
    gluPerspective(fov, _width / _height, near, far);
    glMatrixMode(GL_MODELVIEW);
}
