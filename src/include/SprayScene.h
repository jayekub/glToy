#ifndef _SPRAYSCENE_H_
#define _SPRAYSCENE_H_

#include "Vec.h"
#include "Scene.h"

class Graph;
class Camera;
class Transform;
class SprayEmitter;
class SprayPrim;
class CameraController;
class VecField;

class RenderPass;
class Program;

class SceneRenderVisitor;

// TODO create CameraScene superclass that uses CameraController

class SprayScene : public Scene
{
public:
    SprayScene(int width, int height);
    ~SprayScene();

    void update(double dt);
    void render(RenderPass *renderPass);

    void handleKey(unsigned char key, int x, int y);
    void handleSpecialKey(int key, int x, int y);
    void handleKeyUp(unsigned char key, int x, int y);
    void handleMouse(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);

private:
    ParticleSystem *_particleSystem;
    VecField *_potentialField, *_velocityField;
    SprayEmitter *_sprayEmitter;
    SprayPrim *_sprayPrim;

    Graph *_graph;
    Camera *_camera;
    Transform *_sprayTransform;
    CameraController *_cameraController;

    SceneRenderVisitor *_sceneRenderer;

    Program *_sprayProgram;

    void _build();
    void _updateCurlNoise(double dt);
};

#endif // _SPRAYSCENE_H_
