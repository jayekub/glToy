#ifndef RENDERPASS_H_
#define RENDERPASS_H_

#include "glToy.h"
#include "Vec.h"

class RenderPass
{
public:
    RenderPass(int width, int height);
    virtual ~RenderPass();

    virtual void begin() = 0;
    virtual void end() = 0;

    virtual void setSize(int width, int height);

    int getWidth() { return _width; }
    int getHeight() { return _height; }

    void setFlatProjection();
    void setPerspProjection(float fov = 45., float near = 1.,
                            float far = 1000.);

protected:
    int _width, _height;

    RenderPass() {};
};

#endif /* RENDERPASS_H_ */
