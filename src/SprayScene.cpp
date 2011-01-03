#include "Vec.h"

#include "Graph.h"
#include "Camera.h"
#include "Transform.h"
#include "Spray.h"
#include "CameraController.h"

#include "Program.h"
#include "SceneRenderVisitor.h"

#include "Noise.h"
#include "VecField.h"
#include "VecFieldPrim.h"

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
    delete _cameraController;
    delete _potentialField;
    delete _velocityField;
    delete _sceneRenderer;
}

void SprayScene::update(double dt)
{
    _cameraController->update(dt);
    _spray->update(dt);

    _updateCurlNoise(dt);
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
    _cameraController->handleKey(key, x, y);
}

void SprayScene::handleKeyUp(unsigned char key, int x, int y)
{
    _cameraController->handleKeyUp(key, x, y);
}

void SprayScene::handleMouse(int button, int state, int x, int y)
{
    _cameraController->handleMouse(button, state, x, y);
}

void SprayScene::handleMouseMotion(int x, int y)
{
    _cameraController->handleMouseMotion(x, y);
}

void SprayScene::_build()
{
    //// scene geometry
    _graph = new Graph("sprayScene");

    // Camera
    _camera = new Camera("camera");

    _camera->position = Vec3(0., 0., -30);
    _camera->center = Vec3(0., 0., 0.);
    _camera->up = Vec3(0., 1., 0.);
    _camera->nearClip = 1;
    _camera->farClip = 1000.;//4.;

    _cameraController = new CameraController(_camera);

    _graph->addGlobal(_camera);

    // Spray
    _sprayTransform = new Transform("sprayTransform");

    _sprayTransform->matrix = Mat4::translate(Vec3(-15., -15., -15.));

    _graph->root->addChild(_sprayTransform);

    ////

    _spray = new Spray("spray", Vec3(30., 30., 30.));

    //RandomEmitter *emitter = new RandomEmitter();
    //emitter->emitOnce(5000, 0, 50);

    SprayEmitter *emitter = new SprayEmitter();

    emitter->setPosition(Vec3(15, 15, 15))
            .setRate(100)
            .setDir(Vec3(0, 1, 0))
            .setDirSpread(M_PI)
            .setRadius(50)
            .setRadiusSpread(20)
            .setSpeed(10);

    _spray->addEmitter(emitter);

    _potentialField = new VecField(Vec3(19, 19, 19));
    _velocityField = new VecField(Vec3(30, 30, 30));

    AdvectionField *field = new AdvectionField(_velocityField, 0.9);

    _spray->addField(field);

    _sprayTransform->addChild(_spray);

    //// XXX field debugging
    /*
    Transform *potFieldTransform = new Transform("potFieldTransform");

    potFieldTransform->matrix = Mat4::translate(Vec3(-70, 0, 0));


    Transform *velFieldTransform = new Transform("velFieldTransform");

    velFieldTransform->matrix = Mat4::translate(Vec3(-100, 0, 0));

    _graph->root->addChild(velFieldTransform);
    _graph->root->addChild(potFieldTransform);

    VecFieldPrim *potFieldPrim =
        new VecFieldPrim("potFieldPrim", _potentialField);
    VecFieldPrim *velFieldPrim =
        new VecFieldPrim("velFieldPrim", _velocityField);

    potFieldPrim->setColor(Vec3(0., 0., 1.));

    potFieldTransform->addChild(potFieldPrim);
    velFieldTransform->addChild(velFieldPrim);
    */

    //// rendering

    _sceneRenderer = new SceneRenderVisitor();

    //_updateCurlNoise(0);
}

void SprayScene::_updateCurlNoise(double dt)
{
    static double elapsed = 0;
    elapsed += dt;

    ////

    const Vec3 &potSize = _potentialField->getSize();

    //_potentialField->randomize();

    static const Vec3 xdir = Vec3::randVec(-1, 1).normalize(),
                      xint = Vec3::randVec(-1e4, 1e4),

                      ydir = Vec3::randVec(-1, 1).normalize(),
                      yint = Vec3::randVec(-1e4, 1e4),

                      zdir = Vec3::randVec(-1, 1).normalize(),
                      zint = Vec3::randVec(-1e4, 1e4);

    const double t = elapsed/10.;

    const Vec3 xoff = t*xdir + xint,
               yoff = t*ydir + yint,
               zoff = t*zdir + zint;

    for (int x = 0; x < potSize.x; ++x)
        for (int y = 0; y < potSize.y; ++y)
            for (int z = 0; z < potSize.z; ++z) {
                Vec3 pos(x, y, z),
                     xpos = xoff + pos,
                     ypos = yoff + pos,
                     zpos = zoff + pos;

                (*_potentialField)(x, y, z) =
                    Vec3(Noise::noise3(xpos.x, xpos.y, xpos.z),
                         Noise::noise3(ypos.x, ypos.y, ypos.z),
                         Noise::noise3(zpos.x, zpos.y, zpos.z));
            }

    const Vec3 &velSize = _velocityField->getSize();
    const Vec3 fScale = Vec3(
        (potSize.x - 1.)/(velSize.x - 1.),
        (potSize.y - 1.)/(velSize.y - 1.),
        (potSize.z - 1.)/(velSize.z - 1.));

    for (int x = 0; x < velSize.x; ++x)
        for (int y = 0; y < velSize.y; ++y)
            for (int z = 0; z < velSize.z; ++z)
                (*_velocityField)(x, y, z) =  10.*
                    _potentialField->curl(
                        x*fScale.x, y*fScale.y, z*fScale.z);
}
