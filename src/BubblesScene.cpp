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

// TODO refactor camera control into CameraController class

BubblesScene::BubblesScene(int width, int height) :
    Scene(width, height), _rotationAngle(0.), _dof(true), _grabMouse(false),
    _lastMousePos(width / 2, height / 2)
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
    _sceneRenderer->setRenderPass(_dof ? _geomPass : renderPass);
//    _sceneRenderer->setRenderPass(renderPass);
    _sceneRenderer->setCameraName("camera");
    _sceneRenderer->render(_graph);

    if (_dof) {
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
        case 't':
            _dof = !_dof;
            break;
    }

    _camera->position += translation;
    _camera->center += translation;
}

void BubblesScene::handleMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT && state == GLUT_DOWN) {
        _grabMouse = !_grabMouse;
    }
}

void BubblesScene::handleMouseMotion(int x, int y)
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

void BubblesScene::_build()
{
    //// scene geometry
    _graph = new Graph("bubblesScene");

    // Camera
    _camera = new Camera("camera");

    /*
    _camera->projection = Camera::ORTHO;
    _camera->width = 30.;
    _camera->height = 30.;
    */

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
    _bubbles->emitRandom(5000, 1.5);

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
