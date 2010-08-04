#include "utils.h"

#include "CombineRenderer.h"

CombineRenderer::CombineRenderer(
        const std::vector<TextureRenderPass *> &inputPasses) :
        _inputPasses(inputPasses)
{
    _fractalProgram.addShader(
            Program::ShaderSpec("shaders/combine.fp", GL_FRAGMENT_SHADER));
    _fractalProgram.reload();
}

void CombineRenderer::render(RenderPass *renderPass)
{
    int vpWidth = renderPass->getWidth();
    int vpHeight = renderPass->getHeight();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0.0, 0.0, vpWidth, vpHeight);
    glOrtho(0, vpWidth, vpHeight, 0, 0, 1);

//    renderPass->setFlatProjection();
    _fractalProgram.use();

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _inputPasses[0]->getTexture());
    glUniform1i(_fractalProgram.uniform("tex0"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _inputPasses[1]->getTexture());
    glUniform1i(_fractalProgram.uniform("tex1"), 1);

    drawViewportQuad(renderPass->getWidth(), renderPass->getHeight());
}
