#include <stdio.h>

#include "Renderer.h"

Renderer::Renderer(RenderPass *renderPass) :
    _renderPass(renderPass)
{
}

Renderer::~Renderer()
{
}

void Renderer::render(RenderPass *renderPass)
{
    RenderPass *rp = renderPass ? renderPass : _renderPass;

    if (!rp) {
        fprintf(stderr, "No RenderPass specified for Renderer!\n");
        return;
    }

    rp->begin();

    _render(rp);

    rp->end();
}
