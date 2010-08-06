#include "utils.h"

#include "TextureRenderer.h"

void TextureRenderer::render(RenderPass *renderPass)
{
    int vpWidth = renderPass->getWidth();
    int vpHeight = renderPass->getHeight();

    glUseProgram(0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0.0, 0.0, vpWidth, vpHeight);
    glOrtho(0, vpWidth, vpHeight, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0, 1.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texture);

    drawViewportQuad(renderPass->getWidth(), renderPass->getHeight());

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}
