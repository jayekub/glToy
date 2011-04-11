#ifndef _CAMERASCENE_H_
#define _CAMERASCENE_H_

#include "Scene.h"
#include "Camera.h"
#include "CameraController.h"

class RenderPass;

class CameraScene : public Scene
{
public:
    CameraScene(int width, int height, std::string cameraName = "camera") :
        Scene(width, height),
        _camera(cameraName),
        _cameraController(&_camera) { }

    virtual void render(RenderPass *renderPass) = 0;

    void update(double dt) { _cameraController.update(dt); }

    void handleKey(unsigned char key, int x, int y) {
        _cameraController.handleKey(key, x, y); }

    void handleKeyUp(unsigned char key, int x, int y) {
        _cameraController.handleKeyUp(key, x, y); }

    void handleMouse(int button, int state, int x, int y) {
        _cameraController.handleMouse(button, state, x, y); }

    void handleMouseMotion(int x, int y) {
        _cameraController.handleMouseMotion(x, y); }

protected:
    Camera _camera;
    CameraController _cameraController;
}
