#include "Program.h"
#include "TextureRenderPass.h"
#include "TextureRenderer.h"

#include "ParticleSystem.h"
#include "FluidSimField.h"
#include "CellNoisePrim.h"

#include "CellNoiseScene.h"

CellNoiseScene::CellNoiseScene(int width, int height,
                               float fluidSize) :
    Scene(width, height), _grabMouse(false),
    _lastMousePos(width / 2, height / 2), _fluidSize(fluidSize)
{
    _build();
}

CellNoiseScene::~CellNoiseScene()
{
    delete _particleSystem0;
    delete _particleSystem1;

    delete _cellNoisePrim0;
    delete _cellNoisePrim1;

    delete _cellNoisePass0;
    delete _cellNoisePass1;

    delete _combineRenderer;
    delete _combineProgram;
}

void CellNoiseScene::update(double dt)
{
    _fluidField->update(dt);

    _particleSystem0->update(dt);
    _particleSystem1->update(dt);
}

void CellNoiseScene::render(RenderPass *renderPass)
{
    glViewport(0., 0., _width, _height);

    _cellNoisePass0->begin();
    _renderState.renderPass = _cellNoisePass0;

    glClearColor(1., 1., 1., 1.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _cellNoisePrim0->render(_renderState);

    _cellNoisePass0->end();

    //
    _cellNoisePass1->begin();
    _renderState.renderPass = _cellNoisePass1;

    glClearColor(1., 1., 1., 1.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _cellNoisePrim1->render(_renderState);

    _cellNoisePass1->end();

    //
    _combineRenderer->setRenderPass(renderPass);
    _combineRenderer->render();
}

void CellNoiseScene::resize(int width, int height)
{
    Scene::resize(width, height);

    _renderState.reset();
    _renderState.multTransformMat(Mat4::scale(Vec3(width, height, 1.)));
    _renderState.projectionMat = Mat4::ortho(0., width, height, 0., 0., 1.);
}

void CellNoiseScene::handleMouseMotion(int x, int y)
{
    Vec2 mousePos(x, y);

    Vec2 velocity = mousePos - _lastMousePos;

    float invW = 1. / (float) _width;
    float invH = 1. / (float) _height;

    velocity.x *= invW;
    velocity.y *= invH;
    velocity = velocity * 1000.;

    _fluidField->addForceAtPos((float) x * invW, (float) y * invH,
                                velocity.x, velocity.y);

    _lastMousePos = mousePos;
}

void CellNoiseScene::_build()
{
    _renderState.reset();
    _renderState.multTransformMat(Mat4::scale(Vec3(_width, _height, 1.)));
    _renderState.projectionMat = Mat4::ortho(0., _width, _height, 0., 0., 1.);
    //_renderState.projectionMat = Mat4::ortho(0., 1., 1., 0., 0., 1.);

    _particleSystem0 = new ParticleSystem(Vec3(1., 1., 0.),
                                          ParticleSystem::WRAP);

    _particleSystem1 = new ParticleSystem(Vec3(1., 1., 0.),
                                          ParticleSystem::WRAP);

    _cellNoisePrim0 =
        new CellNoisePrim("cellNoise0", _particleSystem0, .15);

    _cellNoisePrim1 =
        new CellNoisePrim("cellNoise1", _particleSystem1, .15);

    _fluidField = new FluidSimField(_fluidSize);

    _particleSystem0->addField(_fluidField);
    _particleSystem1->addField(_fluidField);

    RandomEmitter *emitter0 = new RandomEmitter();

    emitter0->emitOnce(200, 0.);
    _particleSystem0->addEmitter(emitter0);

    RandomEmitter *emitter1 = new RandomEmitter();

    emitter1->emitOnce(1000, 0.);
    _particleSystem1->addEmitter(emitter1);

    _cellNoisePass0 = new TextureRenderPass(_width, _height);
    _cellNoisePass1 = new TextureRenderPass(_width, _height);

    _combineProgram = new Program();

    _combineProgram->addShader(
        new Program::Shader("shaders/standard.vs", GL_VERTEX_SHADER));
    _combineProgram->addShader(
        new Program::Shader("shaders/combine.fs",
                            GL_FRAGMENT_SHADER));

    _combineProgram->link();

    _combineRenderer = new TextureRenderer();

    _combineRenderer->setProgram(_combineProgram);
    _combineRenderer->setTexture(_cellNoisePass0->getTexture());
    _combineRenderer->addTexture(_cellNoisePass1->getTexture());
}

