#include "Vec.h"

#include "Graph.h"
#include "Camera.h"
#include "CameraController.h"
#include "Transform.h"
#include "RenderPass.h"

#include "WavePrim.h"
#include "SceneRenderVisitor.h"

#include "WaveScene.h"

WaveScene::WaveScene(int width, int height) :
    Scene(width, height)
{
    _build();
}

WaveScene::~WaveScene()
{
    // Graph deletes all registered nodes
    delete _graph;
    delete _cameraController;
    delete _sceneRenderer;
}

void WaveScene::update(double dt)
{
    _cameraController->update(dt);
    _camera->dt = dt;

    _wavePrim->update(dt);
}

void WaveScene::render(RenderPass *renderPass)
{
    _sceneRenderer->setRenderPass(renderPass);
    _sceneRenderer->setCameraName("camera");
    _sceneRenderer->render(_graph);
}

void WaveScene::handleKey(unsigned char key, int x, int y)
{
    _cameraController->handleKey(key, x, y);
}

void WaveScene::handleKeyUp(unsigned char key, int x, int y)
{
    _cameraController->handleKeyUp(key, x, y);
}

void WaveScene::handleMouse(int button, int state, int x, int y)
{
    _cameraController->handleMouse(button, state, x, y);
}

void WaveScene::handleMouseMotion(int x, int y)
{
    _cameraController->handleMouseMotion(x, y);
}

void WaveScene::_build()
{
    //// scene geometry
    _graph = new Graph("waveScene");

    // Camera
    _camera = new Camera("camera");

    _camera->position = Vec3(0., 0., -30);
    _camera->center = Vec3(0., 0., 0.);
    _camera->up = Vec3(0., 1., 0.);
    _camera->nearClip = 1;
    _camera->farClip = 500.;//4.;

    _cameraController = new CameraController(_camera);

    _graph->addGlobal(_camera);

    // Wave
    _waveTransform = new Transform("waveTransform");

    //_waveTransform->matrix = Mat4::translate(Vec3(500, 500, 10));

    _graph->root->addChild(_waveTransform);

    _wavePrim = new WavePrim("wave");

    _waveTransform->addChild(_wavePrim);

    //// rendering

    _sceneRenderer = new SceneRenderVisitor();
}
