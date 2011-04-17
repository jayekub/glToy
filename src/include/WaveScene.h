#ifndef _WAVESCENE_H_
#define _WAVESCENE_H_

#include "Vec.h"
#include "Scene.h"

class Graph;
class Camera;
class Transform;
class WavePrim;
class CameraController;

class RenderPass;
class Program;

class SceneRenderVisitor;

// TODO create CameraScene superclass that uses CameraController

class WaveScene : public Scene
{
public:
    WaveScene(int width, int height);
    ~WaveScene();

    void update(double dt);
    void render(RenderPass *renderPass);

    void handleKey(unsigned char key, int x, int y);
    //void handleSpecialKey(int key, int x, int y);
    void handleKeyUp(unsigned char key, int x, int y);
    void handleMouse(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);

private:
    Graph *_graph;
    Camera *_camera;
    CameraController *_cameraController;
    Transform *_waveTransform;
    WavePrim *_wavePrim;

    SceneRenderVisitor *_sceneRenderer;

    void _build();
};

#endif // _WAVESCENE_H_
