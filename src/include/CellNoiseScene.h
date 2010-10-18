#ifndef _CELLNOISESCENE_H_
#define _CELLNOISESCENE_H_

#include "Vec.h"
#include "RenderState.h"
#include "Scene.h"

class FluidSimField;
class CellNoiseFluid;
class TextureRenderPass;
class TextureRenderer;
class Program;

class CellNoiseScene : public Scene
{
public:
    CellNoiseScene(int width, int height, float fluidSize = 100);
    ~CellNoiseScene();

    void update(double dt);
    void render(RenderPass *renderPass);

    void resize(int width, int height);
//    void handleKey(unsigned char key, int x, int y);
//    void handleMouse(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);

private:
    bool _grabMouse;
    Vec2 _lastMousePos;

    float _fluidSize;
    FluidSimField *_fluidField;

    CellNoiseFluid *_cellNoiseFluid0, *_cellNoiseFluid1;
    TextureRenderPass *_cellNoisePass0, *_cellNoisePass1;

    TextureRenderer *_combineRenderer;
    Program *_combineProgram;

    RenderState _renderState;

    void _build();
};

#endif // _CELLNOISESCENE_H_
