#ifndef RENDERPASS_H_
#define RENDERPASS_H_

#include "glToy.h"
#include "Listener.h"
#include "Vec.h"

class RenderPass : public Listener
{
public:
    RenderPass(int width, int height) { setSize(width, height); }
    virtual ~RenderPass() {}

    virtual void begin() { glViewport(0., 0., _width, _height); };
    virtual void end() {};

    virtual void setSize(int width, int height) {
        _width = width; _height = height; }

    // Listener implementation
    void resize(int width, int height) { setSize(width, height); }

    int getWidth() { return _width; }
    int getHeight() { return _height; }

protected:
    int _width, _height;

    // For use by subclasses that need to control when setSize is called
    RenderPass() {};
};

#endif /* RENDERPASS_H_ */
