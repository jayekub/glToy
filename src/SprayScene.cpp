#include <sstream>

#include "Noise.h"
#include "VecFieldPrim.h"
#include "SprayScene.h"

SprayScene::SprayScene(int width, int height) :
    Scene(width, height)
{
    _build();
}

void SprayScene::update(double dt)
{
    _cameraController->update(dt);
    _particleSystem->update(dt);

    _camera->dt = dt;

    //_updateCurlNoise(dt);
}

void SprayScene::render(const RenderPassConstPtr &renderPass)
{
    _sceneRenderer->setRenderPass(renderPass);
    _sceneRenderer->setCameraName("camera");
    _sceneRenderer->render(_graph);
}

void SprayScene::handleKey(unsigned char key, int x, int y)
{
    switch (key) {
        case '=':
            _sprayEmitter->setDirSpread(
                _sprayEmitter->getDirSpread() + M_PI/180.);
            break;

        case '-':
            _sprayEmitter->setDirSpread(
                _sprayEmitter->getDirSpread() - M_PI/180.);
            break;

        case '+':
            _sprayEmitter->setSpeed(
                _sprayEmitter->getSpeed() + 10);
            break;

        case '_':
            _sprayEmitter->setSpeed(
                _sprayEmitter->getSpeed() - 10);
            break;

        case ']':
            _sprayEmitter->setRate(
                _sprayEmitter->getRate() + 1);
            break;

        case '[':
            _sprayEmitter->setRate(
                _sprayEmitter->getRate() - 1);
            break;
    }

    _cameraController->handleKey(key, x, y);
}

void SprayScene::handleSpecialKey(int key, int x, int y)
{
    Vec3 dir = _sprayEmitter->getDir();

    switch(key) {
        case GLUT_KEY_UP:
            dir = dir.rotate(M_PI/180., Vec3(1, 0, 0));
            break;
        case GLUT_KEY_DOWN:
            dir = dir.rotate(-M_PI/180., Vec3(1, 0, 0));
            break;
        case GLUT_KEY_LEFT:
            dir = dir.rotate(M_PI/180., Vec3(0, 1, 0));
            break;
        case GLUT_KEY_RIGHT:
            dir = dir.rotate(-M_PI/180., Vec3(0, 1, 0));
            break;
    }

    _sprayEmitter->setDir(dir);
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
    _graph = Graph::create("sprayScene");

    // Camera
    _camera = Camera::create("camera");

    _camera->position = Vec3(0., 0., -30);
    _camera->center = Vec3(0., 0., 0.);
    _camera->up = Vec3(0., 1., 0.);
    _camera->nearClip = 1;
    _camera->farClip = 500.;//4.;

    _cameraController = new CameraController(_camera);

    _graph->addGlobal(_camera);

    // Spray
    _sprayTransform = Transform::create("sprayTransform");

    //_sprayTransform->matrix = Mat4::translate(Vec3(500, 500, 10));

    _graph->root->addChild(_sprayTransform);

    ////

    _particleSystem = new ParticleSystem(Vec3(1000, 1000, 1000),
                                         ParticleSystem::KILL);

    //RandomEmitter *emitter = new RandomEmitter();
    //emitter->emitOnce(5000, 0, 50);

    _sprayEmitter = new SprayEmitter();

    _sprayEmitter->setPosition(Vec3(15, 15, 15))
                  .setRate(100)
                  .setDir(Vec3(0, 1, 0))
                  .setDirSpread(M_PI/8.)
                  .setRadius(.5)
                  .setRadiusSpread(.25)
                  .setSpeed(10);

    _particleSystem->addEmitter(_sprayEmitter);

    _particleSystem->addField(new GravityField());

//    _potentialField = new VecField(Vec3(30, 30, 30));
//    _velocityField = new VecField(Vec3(30, 30, 30));

//    AdvectionField *field = new AdvectionField(_velocityField, 0.9);

//    _particleSystem->addField(field);

    _sprayPrim = new SprayPrim("spray", _particleSystem);

    _sprayTransform->addChild(_sprayPrim);

/*
    //// XXX field debugging
    Transform *potFieldTransform = new Transform("potFieldTransform");

    potFieldTransform->matrix = Mat4::translate(Vec3(-70, 0, 0));

    _graph->root->addChild(potFieldTransform);

    VecFieldPrim *potFieldPrim =
        new VecFieldPrim("potFieldPrim", _potentialField);

    potFieldPrim->setColor(Vec3(0., 0., 1.));

    potFieldTransform->addChild(potFieldPrim);

    Transform *velFieldTransform = new Transform("velFieldTransform");

    velFieldTransform->matrix = Mat4::translate(Vec3(-100, 0, 0));

    _graph->root->addChild(velFieldTransform);

    VecFieldPrim *velFieldPrim =
        new VecFieldPrim("velFieldPrim", _velocityField);

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

    //const Vec3 &potSize = _potentialField->getSize();

    _potentialField->randomize();

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

/*
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
    */

    const Vec3 &velSize = _velocityField->getSize();
    /*
    const Vec3 fScale = Vec3(
        (potSize.x - 1.)/(velSize.x - 1.),
        (potSize.y - 1.)/(velSize.y - 1.),
        (potSize.z - 1.)/(velSize.z - 1.));
    */

    for (int x = 0; x < velSize.x; ++x)
        for (int y = 0; y < velSize.y; ++y)
            for (int z = 0; z < velSize.z; ++z) {
                Vec3 pos(x, y, z),
                     xpos = xoff + pos,
                     ypos = Vec3(xpos.y+31.416f, z-47.853f, x+12.793f),
                     zpos = Vec3(xpos.z-233.145f, x-113.408f, y-185.31f);
                     //ypos = yoff + pos,
                     //zpos = zoff + pos;

#if 1
                (*_potentialField)(x, y, z) =
                    Vec3(Noise::vnoise3f(xpos.x, xpos.y, xpos.z),
                         Noise::vnoise3f(ypos.x, ypos.y, ypos.z),
                         Noise::vnoise3f(zpos.x, zpos.y, zpos.z));

                (*_velocityField)(x, y, z) =
                    Vec3(Noise::vnoise3f(xpos.x, xpos.y, xpos.z),
                         Noise::vnoise3f(ypos.x, ypos.y, ypos.z),
                         Noise::vnoise3f(zpos.x, zpos.y, zpos.z));

#else
                float Nx, Ny, Nz;
                Vec3 dNx, dNy, dNz;

                Noise::dvnoise3f(xpos.x, xpos.y, xpos.z,
                                 Nx, dNx.x, dNx.y, dNx.z);

                Noise::dvnoise3f(ypos.x, ypos.y, ypos.z,
                                 Ny, dNy.x, dNy.y, dNy.z);

                Noise::dvnoise3f(zpos.x, zpos.y, zpos.z,
                                 Nz, dNz.x, dNz.y, dNz.z);

                (*_potentialField)(x, y, z) = Vec3(Nx, Ny, Nz);

                (*_velocityField)(x, y, z) =
                    Vec3(dNy.z - dNz.y, dNz.x - dNx.z, dNx.y - dNy.x);

#endif
                /*
                (*_velocityField)(x, y, z) =  10.*
                    //_potentialField->curl(
                    (*_potentialField)(
                        (float) x*fScale.x,
                        (float) y*fScale.y,
                        (float) z*fScale.z);
                    */
            }
}
