#ifndef __CAMERACONTROLLER_H__
#define __CAMERACONTROLLER_H__

#include "Vec.h"

class Camera;

class CameraController
{
public:
    CameraController(Camera *camera);

    void update(double dt);

    void handleKey(unsigned char key, int x, int y);
    void handleKeyUp(unsigned char key, int x, int y);
    void handleMouse(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);
    void handlePassiveMouseMotion(int x, int y);

private:
    Camera *_camera;

    bool _grabMouse;
    Vec2 _lastMousePos;

    float _transScale;
    float _rotScale;

    Vec3 _look;
    Vec2 _rotation;
    Vec3 _deltaTrans;
};

#endif // __CAMERACONTROLLER_H__
