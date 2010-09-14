#ifndef SCREEN2DRENDERPASS_H_
#define SCREEN2DRENDERPASS_H_

#include "glToy.h"

#include "RenderPass.h"

class ScreenRenderPass : public RenderPass
{
public:
    ScreenRenderPass(int width, int height);

    void begin();
    void end();
};

#endif /* SCREEN2DRENDERPASS_H_ */
