#include "utils.h"
#include "Mat.h"
#include "CameraController.h"

CameraController::CameraController(CameraPtr camera) :
    _camera(camera), _grabMouse(false), _transScale(25.), _rotScale(.01)
{
    _look = (_camera->center - _camera->position).normalize();
}

void CameraController::update(double dt)
{
    _camera->position += _deltaTrans*dt;
    _camera->center += _deltaTrans*dt;
}

void CameraController::handleKey(unsigned char key, int x, int y)
{
    Vec3 forward = (_camera->center - _camera->position).normalize();
    Vec3 right = forward.cross(_camera->up).normalize();

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

    _deltaTrans = _transScale*translation;
}

void CameraController::handleKeyUp(unsigned char key, int x, int y)
{
    switch (key) {
        case 'w':
        case 's':
        case 'a':
        case 'd':
            _deltaTrans = Vec3(0., 0., 0.);
            break;
    }
}

void CameraController::handleMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT && state == GLUT_DOWN) {
        _grabMouse = !_grabMouse;
    }
}

void CameraController::handleMouseMotion(int x, int y)
{
    if (_grabMouse) {
        _rotation += _rotScale * (Vec2(x, y) - _lastMousePos);

        _rotation.x = fmod(_rotation.x, 2.*M_PI);
        _rotation.y = CLAMP(_rotation.y, -M_PI/2. + 1e-3, M_PI/2. - 1e-3);

        Mat4 transform = Mat4::rotate(-_rotation.x, Vec3(0., 1., 0.)) *
                         Mat4::rotate(_rotation.y, Vec3(1., 0., 0.));

        _camera->center = transform.vtransform(_look) + _camera->position;

        //_lastMousePos = Vec2(_width / 2, _height / 2);
        //glutWarpPointer(_lastMousePos.x, _lastMousePos.y);
    }

    _lastMousePos = Vec2(x, y);
}

void CameraController::handlePassiveMouseMotion(int x, int y)
{
    handleMouseMotion(x, y);
}
