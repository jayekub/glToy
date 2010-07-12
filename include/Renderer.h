#ifndef RENDERER_H_
#define RENDERER_H_

#include <stddef.h>

#include "RenderPass.h"

class Renderer
{
public:
    Renderer(RenderPass *renderPass = NULL);
    virtual ~Renderer();

    virtual void render(RenderPass *renderPass = NULL);
    virtual void reload() = 0;

protected:
    RenderPass *_renderPass;

    virtual void _render(RenderPass *renderPass) = 0;
};

#endif /* RENDERER_H_ */
