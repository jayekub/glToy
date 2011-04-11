#ifndef __CAMERACONTROLLER_H__
#define __CAMERACONTROLLER_H__

#include "ptrs.h"
#include "Camera.h"
#include "Vec.h"

DEF_SCOPED_PTR(CameraController);

class CameraController
{
public:
    void update(double dt);

    void handleKey(unsigned char key, int x, int y);
    void handleKeyUp(unsigned char key, int x, int y);
    void handleMouse(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);
    void handlePassiveMouseMotion(int x, int y);

    static CameraControllerPtr create(const CameraPtr &camera) {
        return CameraControllerPtr(new CameraController(camera)); }

private:
    CameraPtr _camera;

    bool _grabMouse;
    Vec2 _lastMousePos;

    float _transScale;
    float _rotScale;

    Vec3 _look;
    Vec2 _rotation;
    Vec3 _deltaTrans;

    CameraController(CameraPtr camera);
};

#endif // __CAMERACONTROLLER_H__
