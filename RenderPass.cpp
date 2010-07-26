#include "RenderPass.h"

RenderPass* RenderPass::_currentPass = NULL;

RenderPass::RenderPass(int width, int height)
{
    setSize(width, height);
}

RenderPass::~RenderPass()
{
}

void RenderPass::begin()
{
    _currentPass = this;
}

void RenderPass::end()
{
    _currentPass = NULL;
}

void RenderPass::setSize(int width, int height)
{
    _width = width;
    _height = height;
}

void RenderPass::setFlatProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0.0, 0.0, _width, _height);
    glOrtho(0, _width, _height, 0, 0, 1);
}

void RenderPass::setPerspProjection(float fov, float near, float far)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0.0, 0.0, _width, _height);
    gluPerspective(fov, _width / _height, near, far);
}
