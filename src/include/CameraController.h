#ifndef __CAMERACONTROLLER_H__
#define __CAMERACONTROLLER_H__

#include "Vec.h"

class Camera;

class CameraController
{
public:
    CameraController(Camera *camera);

    void handleKey(unsigned char key, int x, int y);
    void handleMouse(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);

private:
    Camera *_camera;

    bool _grabMouse;
    Vec2 _lastMousePos;
};

#endif // __CAMERACONTROLLER_H__
