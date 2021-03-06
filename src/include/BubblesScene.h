#ifndef _BUBBLESSCENE_H_
#define _BUBBLESSCENE_H_

#include "Vec.h"
#include "Scene.h"

class ParticleSystem;

class Graph;
class Camera;
class Light;
class Transform;
class BubblesPrim;
class CameraController;

class RenderPass;
class TextureRenderPass;
class Program;

class SceneRenderVisitor;
class TextureRenderer;

class BubblesScene : public Scene
{
public:
    BubblesScene(int width, int height);
    ~BubblesScene();

    void update(double dt);
    void render(RenderPass *renderPass);

    void resize(int width, int height);
    void handleKey(unsigned char key, int x, int y);
    void handleKeyUp(unsigned char key, int x, int y);
    void handleMouse(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);

private:
    ParticleSystem *_particleSystem;

    float _rotationAngle;
    bool _dof;

    Graph *_graph;
    Camera *_camera;
    Transform *_bubblesTransform;
    BubblesPrim *_bubblesPrim;
    CameraController *_cameraController;

    TextureRenderPass *_geomPass, *_blurPass1, *_blurPass2;

    Program *_blurProgram, *_dofProgram;

    SceneRenderVisitor *_sceneRenderer;
    TextureRenderer *_textureRenderer;

    void _build();
};

#endif // _BUBBLESSCENE_H_
