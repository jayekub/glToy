#ifndef FRACTALRENDERER_H_
#define FRACTALRENDERER_H_

#include <vector>

#include "glToy.h"
#include "RenderPass.h"
#include "TextureRenderPass.h"
#include "Renderer.h"
#include "Program.h"

class CombineRenderer : public Renderer
{
public:
    CombineRenderer(const std::vector<TextureRenderPass *> &inputPasses);

    void render(RenderPass *renderPass);

private:
    std::vector<TextureRenderPass *> _inputPasses;
    Program _fractalProgram;

;
};

#endif /* FRACTALRENDERER_H_ */
