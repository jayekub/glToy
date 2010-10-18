#include "Vec.h"

#include "Graph.h"
#include "Camera.h"
#include "Transform.h"
#include "Spray.h"

#include "Program.h"
#include "SceneRenderVisitor.h"

#include "SprayScene.h"

SprayScene::SprayScene(int width, int height) :
    Scene(width, height)
{
    _build();
}

SprayScene::~SprayScene()
{
    // Graph deletes all registered nodes
    delete _graph;
    delete _sceneRenderer;
}

void SprayScene::update(double dt)
{
    _spray->update(dt);
}

void SprayScene::render(RenderPass *renderPass)
{
    _sceneRenderer->setRenderPass(renderPass);
    _sceneRenderer->setCameraName("camera");
    _sceneRenderer->render(_graph);
}

void SprayScene::resize(int width, int height)
{
    Scene::resize(width, height);
}

void SprayScene::handleKey(unsigned char key, int x, int y)
{
}

void SprayScene::_build()
{
    //// scene geometry
    _graph = new Graph("sprayScene");

    // Camera
    _camera = new Camera("camera");

    _camera->position = Vec3(0., 0., -20);
    _camera->center = Vec3(0., 0., 0.);
    _camera->up = Vec3(0., 1., 0.);
    _camera->nearClip = 1;
    _camera->farClip = 100.;//4.;

    _graph->addGlobal(_camera);

    // Spray
    _sprayTransform = new Transform("bubblesTransform");

    _graph->root->addChild(_sprayTransform);

    _spray = new Spray("spray", Vec3(30., 30., 30.));
    //_spray->emitRandom(5000, 1.5);

    _sprayTransform->addChild(_spray);

    //// rendering

    _sceneRenderer = new SceneRenderVisitor();
}
