#include "ScreenRenderPass.h"

ScreenRenderPass::ScreenRenderPass(int width, int height) :
    RenderPass(width, height)
{
}

void ScreenRenderPass::begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    RenderPass::begin();
}

void ScreenRenderPass::end()
{
    RenderPass::end();
}
