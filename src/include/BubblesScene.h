#ifndef _BUBBLESSCENE_H_
#define _BUBBLESSCENE_H_

#include "Vec.h"
#include "Scene.h"

class Graph;
class Camera;
class Light;
class Transform;
class Bubbles;

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

private:
    float _rotationAngle;

    Graph *_graph;
    Camera *_camera;
    Transform *_bubblesTransform;
    Bubbles *_bubbles;

    TextureRenderPass *_geomPass, *_blurPass1, *_blurPass2;

    Program *_blurProgram, *_dofProgram;

    SceneRenderVisitor *_sceneRenderer;
    TextureRenderer *_textureRenderer;

    void _build();
    void _translate(const Vec3 &t);
};

#endif // _BUBBLESSCENE_H_