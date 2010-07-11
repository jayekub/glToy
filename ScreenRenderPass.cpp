#include "ScreenRenderPass.h"

ScreenRenderPass::ScreenRenderPass(int width, int height) :
    RenderPass(width, height)
{
}

void ScreenRenderPass::begin()
{
    glViewport(0.0, 0.0, _width, _height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ScreenRenderPass::end()
{
}
