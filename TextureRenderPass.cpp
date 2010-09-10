#include "TextureRenderPass.h"

TextureRenderPass::TextureRenderPass(int width, int height, float scale) :
    _scale(scale)
{
    glGenFramebuffers(1, &_frameBuffer);
    //glGenRenderbuffers(1, &_depthBuffer);
    glGenTextures(1, &_texture);
    glGenTextures(1, &_depthTexture);

    setSize(width, height);
}

TextureRenderPass::~TextureRenderPass()
{
    glDeleteTextures(1, &_texture);
    glDeleteTextures(1, &_depthTexture);
    //glDeleteRenderbuffers(1, &_depthBuffer);
    glDeleteFramebuffers(1, &_frameBuffer);
}

void TextureRenderPass::begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    RenderPass::begin();
}

void TextureRenderPass::end()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    RenderPass::end();
}

void TextureRenderPass::setSize(int width, int height)
{
    RenderPass::setSize(_scale * width, _scale * height);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // color texture
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    // depth texture
    glBindTexture(GL_TEXTURE_2D, _depthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0,
                 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    //glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);

    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           _texture, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           _depthTexture, 0);

    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    //                          GL_RENDERBUFFER, _depthBuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
