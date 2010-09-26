#include <boost/foreach.hpp>

#include "ofxMSAFluidSolver.h"
#include "RenderPass.h"
#include "CellNoiseFluid.h"

CellNoiseFluid::CellNoiseFluid(const char *name, const Vec2 &size,
                               ofxMSAFluidSolver *fluid,
                               float particleSize) :
    ParticleSystem<>(name, Vec3(size, 0.), WRAP, false),
    _fluid(fluid), _particleSize(particleSize)

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

void CellNoiseFluid::update(double dt)
{
    BOOST_FOREACH(Particle<> *p, _particles) {
        // XXX get rid of this stupid class
        ofPoint velocity;

        _fluid->getInfoAtPos(p->position.x / _size.x,
                             p->position.y / _size.y,
                             &velocity, NULL);

        // XXX figure out what this fudge factor is
        p->position.x += velocity.x * dt * 24.0;
        p->position.y += velocity.y * dt * 24.0;

#define WRAP_DIM(p, d) \
    p->position.d += p->position.d < 0. ? \
        _size.d : p->position.d > _size.d ? -_size.d : 0.;

        WRAP_DIM(p, x);
        WRAP_DIM(p, y);

#undef WRAP_DIM
    }
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
                          sizeof(particle_t), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CellNoiseFluid::_postRender(RenderState &state)
{
}

/*
void CellNoiseFluid::render(RenderState &state)
{
    _cellNoiseProgram.use();

    int rpWidth = renderPass->getWidth();
    int rpHeight = renderPass->getHeight();

    float ps = particleSize * rpWidth;
    float ps_2 = ps / 2.0;

    BOOST_FOREACH(Particle<> *p, noiseParticles->getParticles()) {
        float x = p->position.x * rpWidth;
        float y = rpHeight - p->position.y * rpHeight;

        float velScale = 1.0;

        _drawParticle(x, y, ps * velScale);

        // test if we need to draw a wrapped version of this particle also
        // to make the noise tileable
        double wx = x, wy = y;

        if (x < ps_2) {
            wx = x + rpWidth;
            _drawParticle(wx, y, ps * velScale);
        } else if (x > rpWidth - ps_2) {
            wx = x - rpWidth;
            _drawParticle(wx, y, ps * velScale);
        }

        if (y < ps_2) {
            wy = y + rpHeight;
            _drawParticle(x, wy, ps * velScale);
        } else if (y > rpHeight - ps_2) {
            wy = y - rpHeight;
            _drawParticle(x, wy, ps * velScale);
        }

        if (wx != x && wy != y)
            _drawParticle(wx, wy, ps * velScale);
    }
}
*/
