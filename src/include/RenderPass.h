#ifndef _RENDERPASS_H_
#define _RENDERPASS_H_

#include "ptr.h"
#include "glToy.h"
#include "Listener.h"
#include "Vec.h"

DEF_SHARED_PTR(RenderPass);

class RenderPass : public Listener
{
public:
    virtual void begin() const { glViewport(0., 0., _width, _height); };
    virtual void end() const { };

    virtual void setSize(int width, int height) {
        _width = width; _height = height; }

    // Listener implementation
    void resize(int width, int height) { setSize(width, height); }

    int getWidth() const { return _width; }
    int getHeight() const { return _height; }

protected:
    int _width, _height;

    // not meant to be instantiated directly
    RenderPass(int width, int height) { setSize(width, height); }

    // For use by subclasses that need to control when setSize is called
    RenderPass() { };
};

#endif // _RENDERPASS_H_
