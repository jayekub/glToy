#include "utils.h"

#include "TextureRenderer.h"

void TextureRenderer::render(RenderPass *renderPass)
{
    int vpWidth = renderPass->getWidth();
    int vpHeight = renderPass->getHeight();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0.0, 0.0, vpWidth, vpHeight);
    glOrtho(0, vpWidth, vpHeight, 0, 0, 1);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);

    drawViewportQuad(renderPass->getWidth(), renderPass->getHeight());
}
