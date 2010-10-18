#include <boost/foreach.hpp>

#include "ofxMSAFluidSolver.h"
#include "RenderPass.h"
#include "Particle.h"

#include "CellNoiseFluid.h"

CellNoiseFluid::CellNoiseFluid(const char *name, const Vec2 &size,
                               float particleSize) :
    ParticleSystem(name, Vec3(size, 0.), WRAP, false),
    _particleSize(particleSize)

{
    _cellNoiseProgram.addShader(
        new Program::Shader("shaders/cellnoise.vs", GL_VERTEX_SHADER));

    _cellNoiseProgram.addShader(
        (new Program::Shader(GL_GEOMETRY_SHADER))
            ->addFile("shaders/common.inc")
             .addFile("shaders/cellnoise.gs").compile());

    _cellNoiseProgram.addShader(
        new Program::Shader("shaders/cellnoise.fs", GL_FRAGMENT_SHADER));

    _cellNoiseProgram.link();
}

void CellNoiseFluid::_preRender(RenderState &state)
{
    _cellNoiseProgram.use();

    _cellNoiseProgram.setUniform("modelMat", state.getTransformMat())
                     .setUniform("viewMat", state.viewMat)
                     .setUniform("projMat", state.projectionMat)
                     .setUniform("rasterSize",
                                 Vec2(state.renderPass->getWidth(),
                                      state.renderPass->getHeight()))
                     .setUniform("particleSize", _particleSize);

    const GLuint centerInLoc = _cellNoiseProgram.attribute("centerIn");

    glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer);

    glEnableVertexAttribArray(centerInLoc);

    glVertexAttribPointer(centerInLoc, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Particle), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CellNoiseFluid::_postRender(RenderState &state)
{
}
