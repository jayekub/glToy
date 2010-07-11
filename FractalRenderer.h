#ifndef FRACTALRENDERER_H_
#define FRACTALRENDERER_H_

#include <vector>

#include "glToy.h"
#include "RenderPass.h"
#include "TextureRenderPass.h"
#include "Renderer.h"
#include "Program.h"

class FractalRenderer : public Renderer
{
public:
    FractalRenderer(const std::vector<TextureRenderPass *> &inputPasses,
                    RenderPass *renderPass = NULL);

    void reload();

private:
    std::vector<TextureRenderPass *> _inputPasses;
    Program _fractalProgram;

    virtual void _render(RenderPass *renderPass);
};

#endif /* FRACTALRENDERER_H_ */
