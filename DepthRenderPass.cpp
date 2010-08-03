#include "DepthRenderPass.h"

DepthRenderPass::DepthRenderPass(int width, int height, float resMult)
{
    glGenFramebuffers(1, &_frameBuffer);

    setSize(width, height, resMult);
}

DepthRenderPass::~DepthRenderPass()
{
    glDeleteTextures(1, &_texture);
    glDeleteFramebuffers(1, &_frameBuffer);
}

void DepthRenderPass::begin()
{
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}

void DepthRenderPass::end()
{
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void DepthRenderPass::setSize(int width, int height, float resMult)
{
    _resMult = resMult;
    RenderPass::setSize(width * _resMult, height * _resMult);

    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

    glDeleteTextures(1, &_texture);
    glGenTextures(1, &_texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0,
                 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           _texture, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
