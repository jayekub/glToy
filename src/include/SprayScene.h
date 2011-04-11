#ifndef _SPRAYSCENE_H_
#define _SPRAYSCENE_H_

#include "Vec.h"
#include "Program.h"
#include "RenderPass.h"
#include "Graph.h"
#include "Scene.h"
#include "SceneRenderer.h"
#include "Camera.h"
#include "Transform.h"
#include "CameraController.h"
#include "ParticleSystem.h"
#include "Emitter.h"
#include "VecField.h"
#include "SprayPrim.h"

// TODO create CameraScene superclass that uses CameraController

class SprayScene : public Scene
{
public:
    SprayScene(int width, int height);

    void update(double dt);
    void render(const RenderPassConstPtr &renderPass);

    void handleKey(unsigned char key, int x, int y);
    void handleSpecialKey(int key, int x, int y);
    void handleKeyUp(unsigned char key, int x, int y);
    void handleMouse(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);

private:
    ParticleSystemPtr _particleSystem;
    VecFieldPtr _potentialField, _velocityField;
    SprayEmitterPtr _sprayEmitter;
    SprayPrimPtr _sprayPrim;

    GraphPtr _graph;
    CameraPtr _camera;
    TransformPtr _sprayTransform;
    CameraControllerPtr _cameraController;

    SceneRendererPtr _sceneRenderer;

    ProgramPtr _sprayProgram;

    void _build();
    void _updateCurlNoise(double dt);
};

#endif // _SPRAYSCENE_H_
