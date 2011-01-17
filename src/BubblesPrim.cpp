#include "glToy.h"
#include "Noise.h"
#include "Camera.h"
#include "Particle.h"
#include "BubblesPrim.h"

BubblesPrim::BubblesPrim(
    const char *name,
    ParticleSystem *particleSystem) :
    ParticleSystemPrim(name, particleSystem, true /* needsDepthSort */)
{
    _bubblesProgram.addShader(
        new Program::Shader("shaders/particlequad.vs", GL_VERTEX_SHADER));

    _bubblesProgram.addShader(
        (new Program::Shader(GL_GEOMETRY_SHADER))
            ->addFile("shaders/common.inc")
             .addFile("shaders/particlequad.gs").compile());

    _bubblesProgram.addShader(
        ((new Program::Shader(GL_FRAGMENT_SHADER))
        ->addFile("shaders/common.inc")
         .addFile("shaders/sphere.inc")
         .addFile("shaders/bubbles.fs").compile()));

    _bubblesProgram.link();

    //_bubblesProgram.setDebug(true);
}

void BubblesPrim::_preRender(RenderState &state)
{
    // XXX fragile
    //const Light *firstLight = state.lights.begin()->second;
    const Vec3 &firstLightPos = state.lightPositions.begin()->second;

    _bubblesProgram.use();
    _bubblesProgram.setUniform("modelMat", state.getTransformMat())
                   .setUniform("viewMat", state.viewMat)
                   .setUniform("projMat", state.projectionMat)
                   .setUniform("cameraPos", state.camera->position)
                   .setUniform("lightPos", firstLightPos);

    const GLuint centerInLoc = _bubblesProgram.attribute("centerIn");
    const GLuint radiusInLoc = _bubblesProgram.attribute("radiusIn");

    glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer);

    glEnableVertexAttribArray(centerInLoc);

    glVertexAttribPointer(centerInLoc, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Particle), 0);

    glEnableVertexAttribArray(radiusInLoc);

    glVertexAttribPointer(radiusInLoc, 1, GL_FLOAT, GL_FALSE,
                          sizeof(Particle),
                          BUFFER_OFFSET(2 * sizeof(Vec3)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void BubblesPrim::_postRender(RenderState &state)
{
    glDisable(GL_BLEND);
}
