#include "RenderPass.h"

RenderPass::RenderPass(int width, int height)
{
    setSize(width, height);
}

RenderPass::~RenderPass()
{
}

void RenderPass::setSize(int width, int height)
{
    _width = width;
    _height = height;
}
