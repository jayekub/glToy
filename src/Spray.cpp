#include "glToy.h"
#include "Noise.h"
#include "Camera.h"
#include "Particle.h"
#include "Spray.h"

Spray::Spray(
    const char *name,
    const Vec3 &size) :
    ParticleSystem(name, size, KILL, true)
{
    _sprayProgram.addShader(
        (new Program::Shader(GL_VERTEX_SHADER))
            ->addFile("shaders/common.inc")
             .addFile("shaders/spray.vs").compile());

    /*
    _sprayProgram.addShader(
        (new Program::Shader(GL_GEOMETRY_SHADER))
            ->addFile("shaders/common.inc")
             //.addFile("shaders/sphere.inc")
             //.addFile("shaders/noise.inc")
             .addFile("shaders/spray.gs").compile());
    */

    _sprayProgram.addShader(
        ((new Program::Shader(GL_FRAGMENT_SHADER))
        ->addFile("shaders/common.inc")
         .addFile("shaders/spray.fs").compile()));

    _sprayProgram.link();
}

Spray::~Spray()
{
}

void Spray::_preRender(RenderState &state)
{
    _sprayProgram.use();
    _sprayProgram.setUniform("modelMat", state.getTransformMat())
                  .setUniform("viewMat", state.viewMat)
                  .setUniform("projMat", state.projectionMat)
                  .setUniform("cameraPos", state.camera->position);

    const GLuint centerInLoc = _sprayProgram.attribute("centerIn");
    const GLuint velocityInLoc = _sprayProgram.attribute("velocityIn");
    const GLuint radiusInLoc = _sprayProgram.attribute("radiusIn");

    glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer);

    glEnableVertexAttribArray(centerInLoc);

    glVertexAttribPointer(centerInLoc, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Particle), 0);

    glEnableVertexAttribArray(velocityInLoc);

    glVertexAttribPointer(velocityInLoc, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Particle),
                          BUFFER_OFFSET(sizeof(Vec3)));

    glEnableVertexAttribArray(radiusInLoc);

    glVertexAttribPointer(radiusInLoc, 1, GL_FLOAT, GL_FALSE,
                          sizeof(Particle),
                          BUFFER_OFFSET(2 * sizeof(Vec3)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Spray::_postRender(RenderState &state)
{
    glDisable(GL_BLEND);
    glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
}
