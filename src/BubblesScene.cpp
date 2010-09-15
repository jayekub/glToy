#include "Vec.h"

#include "Graph.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Bubbles.h"

#include "TextureRenderPass.h"
#include "Program.h"

#include "SceneRenderVisitor.h"
#include "TextureRenderer.h"

#include "BubblesScene.h"

BubblesScene::BubblesScene(int width, int height) :
    Scene(width, height), _rotationAngle(0.)
{
    _build();
}

BubblesScene::~BubblesScene()
{
    // Graph deletes all registered nodes
    delete _graph;

    delete _geomPass;
    delete _blurPass1;
    delete _blurPass2;

    delete _sceneRenderer;
    delete _textureRenderer;
    delete _dofProgram;
}

void BubblesScene::update(double dt)
{
    _bubbles->update(dt);

    _bubblesTransform->matrix *= Mat4::rotate(0.25 * dt, Vec3(0., 1., 0.));
}

void BubblesScene::render(RenderPass *renderPass)
{
    _sceneRenderer->setRenderPass(_geomPass);
//    _sceneRenderer->setRenderPass(renderPass);
    _sceneRenderer->setCameraName("camera");
    _sceneRenderer->render(_graph);

    _blurProgram->use();
    _blurProgram->setUniform("horizontal", true);

    _textureRenderer->setRenderPass(_blurPass1);
//    _textureRenderer->setRenderPass(renderPass);
    _textureRenderer->setProgram(_blurProgram);
    _textureRenderer->setTexture(_geomPass->getTexture());
    _textureRenderer->render();

    _blurProgram->setUniform("horizontal", false);

    _textureRenderer->setRenderPass(_blurPass2);
//    _textureRenderer->setRenderPass(renderPass);
//    _textureRenderer->setProgram(_blurProgram);
    _textureRenderer->setTexture(_blurPass1->getTexture());
    _textureRenderer->render();

    _textureRenderer->setRenderPass(renderPass);
    _textureRenderer->setProgram(_dofProgram);
//    _textureRenderer->setTexture(_blurPass1->getTexture());
    _textureRenderer->setTexture(_geomPass->getTexture());
    _textureRenderer->addTexture(_geomPass->getDepthTexture());
    _textureRenderer->addTexture(_blurPass2->getTexture());
    _textureRenderer->render();
}

void BubblesScene::resize(int width, int height)
{
    Scene::resize(width, height);

    _blurProgram->use();
    _blurProgram->setUniform("dxdy",
        Vec2(1. / (float) width, 1. / (float) height));
}

void BubblesScene::handleKey(unsigned char key, int x, int y)
{
    switch (key) {
        case 'w':
            _translate(Vec3(0., 0., 1.));
            break;
        case 's':
            _translate(Vec3(0., 0., -1.));
            break;
        case 'a':
            _translate(Vec3(1., 0., 0.));
            break;
        case 'd':
            _translate(Vec3(-1., 0., 0.));
            break;
    }
}

void BubblesScene::_build()
{
    //// scene geometry
    _graph = new Graph("bubblesScene");

    // Camera
    _camera = new Camera("camera");

    _camera->position = Vec3(0., 0., -20);
    _camera->center = Vec3(0., 0., 0.);
    _camera->up = Vec3(0., 1., 0.);
    _camera->nearClip = 1;
    _camera->farClip = 100.;//4.;

    _graph->addGlobal(_camera);

    // Light
    Transform *keyLightTransform = new Transform("keyLightTransform");

    keyLightTransform->matrix = Mat4::translate(Vec3(5., 5., 3.));

    _graph->addGlobal(keyLightTransform);

    Light *keyLight = new Light("keyLight");

    keyLight->type = Light::POINT;
    keyLight->color = Vec3(1., 1., 1.);

    keyLightTransform->addChild(keyLight);

    // Bubbles
    _bubblesTransform = new Transform("bubblesTransform");

    //_bubblesTransform->matrix = Mat4::translate(Vec3(-15., -15., 3.));

    _graph->root->addChild(_bubblesTransform);

    // center the bubbles container at origin
    Transform *bubblesOffset = new Transform("bubblesOffset");

    bubblesOffset->matrix = Mat4::translate(Vec3(-15., -15., -15.));

    _bubblesTransform->addChild(bubblesOffset);

    _bubbles = new Bubbles("bubbles", Vec3(30., 30., 30.), 0.5); //, true);
    _bubbles->emitRandom(5000, 0.05);

    bubblesOffset->addChild(_bubbles);

    //// rendering

    _geomPass = new TextureRenderPass(_width, _height);
    _blurPass1 = new TextureRenderPass(_width, _height); //, 0.25);
    _blurPass2 = new TextureRenderPass(_width, _height); //, 0.25);

    _sceneRenderer = new SceneRenderVisitor();
    _textureRenderer = new TextureRenderer();

    _blurProgram = new Program();

    _blurProgram->addShader(
         new Program::Shader("shaders/standard.vs", GL_VERTEX_SHADER));
    _blurProgram->addShader(
         new Program::Shader("shaders/blur.fs", GL_FRAGMENT_SHADER));
    _blurProgram->link();

    _blurProgram->use();
    _blurProgram->setUniform("blurWeights", makeBlurWeights(10));
    _blurProgram->setUniform("dxdy",
        Vec2(1. / (float) _blurPass1->getWidth(),
             1. / (float) _blurPass1->getHeight()));

    _dofProgram = new Program();

    _dofProgram->addShader(
         new Program::Shader("shaders/standard.vs", GL_VERTEX_SHADER));
    _dofProgram->addShader(
         new Program::Shader("shaders/dof.fs", GL_FRAGMENT_SHADER));
    _dofProgram->link();
}

void BubblesScene::_translate(const Vec3 &t)
{
    _camera->position += 0.25 * t;
    _camera->center += 0.25 * t;
}
