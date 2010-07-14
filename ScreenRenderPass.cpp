#include "ScreenRenderPass.h"

ScreenRenderPass::ScreenRenderPass(int width, int height) :
    RenderPass(width, height)
{
}

void ScreenRenderPass::begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ScreenRenderPass::end()
{
}
