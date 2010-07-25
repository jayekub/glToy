#include <boost/foreach.hpp>

#include "SceneRenderer.h"

SceneRenderer::SceneRenderer()
{
}

SceneRenderer::~SceneRenderer()
{
    BOOST_FOREACH(Prim *p, _prims) {
        delete p;
    }
}

void
SceneRenderer::render(RenderPass *renderPass)
{
    renderPass->setPerspProjection();

    glLoadIdentity();
    gluLookAt(0., 0., -10., // eye
              0., 0., 0., // center
              0., 1., 0.); // up

    glClearColor(0., 0., 0., 1.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    BOOST_FOREACH(Prim *p, _prims) {
        p->render();
    }
}
