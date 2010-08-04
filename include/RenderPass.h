#ifndef RENDERPASS_H_
#define RENDERPASS_H_

#include "glToy.h"
#include "Listener.h"
#include "Vec.h"

class RenderPass : public Listener
{
public:
    RenderPass(int width, int height);
    virtual ~RenderPass();

    virtual void begin() {};
    virtual void end() {};

    virtual void setSize(int width, int height);

    // Listener implementation
    void resize(int width, int height) { setSize(width, height); }

    int getWidth() { return _width; }
    int getHeight() { return _height; }

protected:
    int _width, _height;

    RenderPass() {};
};

#endif /* RENDERPASS_H_ */
