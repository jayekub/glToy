#include "utils.h"

#include "CombineRenderer.h"

CombineRenderer::CombineRenderer(
        const std::vector<TextureRenderPass *> &inputPasses) :
        _inputPasses(inputPasses)
{
    std::vector<Program::ShaderSpec> shaders;
    shaders.push_back(Program::ShaderSpec("fractal.fp", GL_FRAGMENT_SHADER));

    _fractalProgram.setShaders(shaders, false);
    reload();
}

void CombineRenderer::render(RenderPass *renderPass)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _fractalProgram.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _inputPasses[0]->getTexture());
    glUniform1i(_fractalProgram.uniform("tex0"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _inputPasses[1]->getTexture());
    glUniform1i(_fractalProgram.uniform("tex1"), 1);

    drawViewportQuad(renderPass->getWidth(), renderPass->getHeight());
}

void CombineRenderer::reload()
{
    _fractalProgram.reload();
}
