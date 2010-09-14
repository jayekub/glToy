#ifndef RENDERER_H_
#define RENDERER_H_

#include "RenderPass.h"

class Renderer
{
public:
    virtual void render() {};

    Renderer &setRenderPass(RenderPass *renderPass) {
        _renderPass = renderPass; return *this; }

protected:
    RenderPass *_renderPass;
};

#endif /* RENDERER_H_ */
