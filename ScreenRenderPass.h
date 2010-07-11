#ifndef SCREENRENDERPASS_H_
#define SCREENRENDERPASS_H_

#include "glToy.h"

#include "RenderPass.h"

class ScreenRenderPass : public RenderPass
{
public:
    ScreenRenderPass(int width, int height);

    void begin();
    void end();
};

#endif /* SCREENRENDERPASS_H_ */
