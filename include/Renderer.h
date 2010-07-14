#ifndef RENDERER_H_
#define RENDERER_H_

#include "RenderPass.h"

class Renderer
{
public:
    //virtual void render(RenderPass *renderPass) = 0;
    virtual void reload() = 0;
};

#endif /* RENDERER_H_ */
