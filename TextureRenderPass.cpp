#include "TextureRenderPass.h"

TextureRenderPass::TextureRenderPass(int width, int height)
{
    glGenFramebuffers(1, &_frameBuffer);
    glGenRenderbuffers(1, &_depthBuffer);
//    glGenTextures(1, &_texture);

    setSize(width, height);
}

TextureRenderPass::~TextureRenderPass()
{
    glDeleteTextures(1, &_texture);
    glDeleteRenderbuffers(1, &_depthBuffer);
    glDeleteFramebuffers(1, &_frameBuffer);
}

void TextureRenderPass::begin()
{
    glViewport(0.0, 0.0, _width, _height);
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
    RenderPass::setSize(width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

    //glDeleteTextures(1, &_texture);
    glGenTextures(1, &_texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);


    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           _texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, _depthBuffer);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
