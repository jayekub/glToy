#include "Camera.h"
#include "CameraController.h"

CameraController::CameraController(Camera *camera) :
    _camera(camera), _grabMouse(false)
{
}

void CameraController::handleKey(unsigned char key, int x, int y)
{
    Vec3 forward = 0.25 * (_camera->center - _camera->position).normalize();
    Vec3 right = 0.25 * forward.cross(_camera->up).normalize();

    Vec3 translation;

    switch (key) {
        case 'w':
            translation = forward;
            break;
        case 's':
            translation = -1. * forward;
            break;
        case 'a':
            translation = -1. * right;
            break;
        case 'd':
            translation = right;
            break;
    }

    _camera->position += translation;
    _camera->center += translation;
}

void CameraController::handleMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT && state == GLUT_DOWN) {
        _grabMouse = !_grabMouse;
    }
}

void CameraController::handleMouseMotion(int x, int y)
{
    // XXX broken
    if (_grabMouse) {
        Vec2 delta = 0.01 * (Vec2(x, y) - _lastMousePos);

        Mat4 transform = Mat4::translate(-1. * _camera->position) *
                         Mat4::rotate(delta.x, Vec3(0., 1., 0.)) *
                         Mat4::rotate(delta.y, Vec3(1., 0., 0.)) *
                         Mat4::translate(_camera->position);

        _camera->center = transform.ptransform(_camera->center);
        _camera->up = transform.vtransform(_camera->up);

        _lastMousePos = Vec2(x, y);
        //_lastMousePos = Vec2(_width / 2, _height / 2);
        //glutWarpPointer(_lastMousePos.x, _lastMousePos.y);
    } else {
        _lastMousePos = Vec2(x, y);
    }
}
