#ifndef SCENERENDERER_H_
#define SCENERENDERER_H_

#include <vector>

#include "Renderer.h"
#include "RenderPass.h"
#include "Prim.h"

class SceneRenderer : public Renderer
{
public:
    SceneRenderer();
    virtual ~SceneRenderer();

    void reload() {};
    void render(RenderPass *renderPass);

    void addPrim(Prim *prim) { _prims.push_back(prim); }
    std::vector<Prim *> &getPrims() { return _prims; }

private:
    std::vector<Prim *> _prims;
};

#endif /* SCENERENDERER_H_ */
