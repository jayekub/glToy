#include <stdio.h>

#include "DepthRenderPass.h"

DepthRenderPass::DepthRenderPass(int width, int height, float resMult)
{
    glGenFramebuffers(1, &_frameBuffer);
    glGenTextures(1, &_texture);

    setSize(width, height, resMult);
}

DepthRenderPass::~DepthRenderPass()
{
    glDeleteTextures(1, &_texture);
    glDeleteFramebuffers(1, &_frameBuffer);
}

void DepthRenderPass::begin()
{
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glClearDepth(0.5);

    glViewport(0.0, 0.0, _width, _height);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

    RenderPass::begin();
}

void DepthRenderPass::end()
{
    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    RenderPass::end();
}

void DepthRenderPass::setSize(int width, int height, float resMult)
{
    _resMult = resMult;
    RenderPass::setSize(width * _resMult, height * _resMult);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0,
                 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           _texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        fprintf(stderr, "WTF!!!\n");

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
