#ifndef _SPRAYSCENE_H_
#define _SPRAYSCENE_H_

#include "Vec.h"
#include "Scene.h"

class Graph;
class Camera;
class Transform;
class Spray;
class CameraController;

class RenderPass;
class Program;

class SceneRenderVisitor;

class SprayScene : public Scene
{
public:
    SprayScene(int width, int height);
    ~SprayScene();

    void update(double dt);
    void render(RenderPass *renderPass);

    void resize(int width, int height);
    void handleKey(unsigned char key, int x, int y);
    void handleMouse(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);

private:
    Graph *_graph;
    Camera *_camera;
    Transform *_sprayTransform;
    Spray *_spray;
    CameraController *_cameraController;

    Program *_sprayProgram;

    SceneRenderVisitor *_sceneRenderer;

    void _build();
};

#endif // _SPRAYSCENE_H_
