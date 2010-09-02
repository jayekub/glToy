#include "glToy.h"
#include "Noise.h"
#include "Camera.h"
#include "Bubbles.h"

Bubbles::Bubbles(const char *name, const Vec3 &size,
                 float radius, int numLat, int numLong) :
    ParticleSystem<Vec3>(name, size),
    _radius(radius), _numLat(numLat), _numLong(numLong)
{
    _bubblesProgram.addShader(
        new Program::Shader("shaders/vertexid.vs", GL_VERTEX_SHADER));

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

    //_permTexture = GLSLNoise::makePermutationTexture();
    //_gradTexture = GLSLNoise::makeGradientTexture();
}

Bubbles::~Bubbles()
{
}

void Bubbles::_prepRender(const RenderState &state)
{
    // XXX fragile
    //const Light *firstLight = state.lights.begin()->second;
    const Vec3 &firstLightPos = state.lightPositions.begin()->second;

    Mat4 modelMat = state.getTransformMat() * Mat4::scale(_size);

    _bubblesProgram.use();
    _bubblesProgram.setUniform("modelMat", modelMat)
                  .setUniform("viewMat", state.viewMat)
                  .setUniform("projMat", state.projectionMat)
                  .setUniform("cameraPos", state.camera->position)
                  .setUniform("lightPos", firstLightPos)
                  .setUniform("numLat", _numLat)
                  .setUniform("numLong", _numLong)
                  .setUniform("radius", _radius)
                  .resetSamplers()
                  .setSampler("permTexture", GL_TEXTURE_2D, _permTexture)
                  .setSampler("gradTexture", GL_TEXTURE_2D, _gradTexture);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

    glEnableVertexAttribArray(_bubblesProgram.attribute("vertexIn"));
    glVertexAttribPointer(_bubblesProgram.attribute("vertexIn"), 3, GL_FLOAT,
                          GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

