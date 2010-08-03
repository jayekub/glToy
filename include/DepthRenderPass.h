#ifndef DEPTHRENDERPASS_H_
#define DEPTHRENDERPASS_H_

#include "TextureRenderPass.h"

class DepthRenderPass : public TextureRenderPass
{
public:
    DepthRenderPass(int width, int height, float resMult = 1.);
    virtual ~DepthRenderPass();

    virtual void begin();
    virtual void end();

    void setSize(int width, int height, float resMult = 1.);

    // XXX is this actually necessary?
    void setSize(int width, int height) { setSize(width, height, 1.); }

protected:
    float _resMult;
};

#endif /* DEPTHRENDERPASS_H_ */
