#include "glToy.h"
#include "Noise.h"
#include "Camera.h"
#include "Bubbles.h"

Bubbles::Bubbles(
    const char *name,
    const Vec3 &size,
    float radius,
    bool drawBox) :
    ParticleSystem<particle_t>(name, size, true /* needsDepthSort */),
    _radius(radius), _drawBox(drawBox)
{
    _bubblesProgram.addShader(
        new Program::Shader("shaders/bubbles.vs", GL_VERTEX_SHADER));

    _bubblesProgram.addShader(
        (new Program::Shader(GL_GEOMETRY_SHADER))
            ->addFile("shaders/common.inc")
             //.addFile("shaders/sphere.inc")
             //.addFile("shaders/noise.inc")
             .addFile("shaders/bubbles.gs").compile());

    _bubblesProgram.addShader(
        ((new Program::Shader(GL_FRAGMENT_SHADER))
        ->addFile("shaders/common.inc")
         .addFile("shaders/sphere.inc")
         .addFile("shaders/bubbles.fs").compile()));

    _bubblesProgram.link();

    //_bubblesProgram.setDebug(true);

    ////

    _boxProgram.addShader(
       new Program::Shader("shaders/standard.vs", GL_VERTEX_SHADER));

    _boxProgram.addShader(
       new Program::Shader("shaders/box.gs", GL_GEOMETRY_SHADER));

    _boxProgram.addShader(
       new Program::Shader("shaders/constant.fs", GL_FRAGMENT_SHADER));

    _boxProgram.link();

    //_permTexture = GLSLNoise::makePermutationTexture();
    //_gradTexture = GLSLNoise::makeGradientTexture();
}

Bubbles::~Bubbles()
{
}

// XXX refactor to Emitter
void Bubbles::_setRandomAttributes(particle_t *p) const {
    p->attributes.radius = _radius * (randFloat() + 0.5);
}

void Bubbles::_preRender(RenderState &state)
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
                          sizeof(particle_t), 0);

    glEnableVertexAttribArray(radiusInLoc);

    glVertexAttribPointer(radiusInLoc, 1, GL_FLOAT, GL_FALSE,
                          sizeof(particle_t),
                          BUFFER_OFFSET(2 * sizeof(Vec3)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Bubbles::_postRender(RenderState &state)
{
    glDisable(GL_BLEND);

    if (_drawBox) {
        /* XXX gl3
        _boxProgram.use();
        _boxProgram.setUniform("modelMat", state.getTransformMat())
                   .setUniform("viewMat", state.viewMat)
                   .setUniform("projMat", state.projectionMat);

        */
    }
}
