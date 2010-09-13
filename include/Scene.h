#ifndef _SCENE_H_
#define _SCENE_H_

#include "Listener.h"

class RenderPass;

class Scene
{
public:
    Scene(int width, int height) : _width(width), _height(height) {}
    virtual ~Scene() {};

    virtual void update(double dt) = 0;
    virtual void render(RenderPass *renderPass) = 0;

    void resize(int width, int height) { _width = width; _height = height; }

    virtual void handleKey(unsigned char key, int x, int y) = 0;

protected:
    int _width, _height;
};

#endif // _SCENE_H_
