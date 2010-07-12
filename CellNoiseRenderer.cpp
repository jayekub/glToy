#include <math.h>

#include <boost/foreach.hpp>

#include "utils.h"
#include "CellNoiseRenderer.h"

CellNoiseRenderer::CellNoiseRenderer(
        ParticleSystem *noiseParticles,
        double particleSize) :

        _noiseParticles(noiseParticles), _particleSize(particleSize) {

    std::vector<Program::ShaderSpec> shaders;
    shaders.push_back(Program::ShaderSpec("cellNoise.fp", GL_FRAGMENT_SHADER));

    _cellNoiseProgram.setShaders(shaders);
}

void CellNoiseRenderer::render(
        RenderPass *renderPass,
        ParticleSystem *noiseParticles,
        double particleSize)
{
    ParticleSystem *oldNoiseParticles = _noiseParticles;
    double oldParticleSize = _particleSize;

    if (noiseParticles) _noiseParticles = noiseParticles;
    if (particleSize > 0.0) _particleSize = particleSize;

    Renderer::render(renderPass);

    _noiseParticles = oldNoiseParticles;
    _particleSize = oldParticleSize;
}

void CellNoiseRenderer::reload()
{
    _cellNoiseProgram.reload();
}

void CellNoiseRenderer::_render(RenderPass *renderPass) {
    int rpWidth = renderPass->getWidth();
    int rpHeight = renderPass->getHeight();

    double ps_2 = _particleSize / 2.0;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _cellNoiseProgram.use();

    BOOST_FOREACH(Particle *p, _noiseParticles->getParticles()) {
        double x = p->position.x * rpWidth;
        double y = p->position.y * rpHeight;

        _drawParticle(x, y);

        // test if we need to draw a wrapped version of this particle also
        // to make the noise tileable
        double wx = x, wy = y;

        if (x < ps_2) {
            wx = x + rpWidth;
            _drawParticle(wx, y);
        } else if (x > rpWidth - ps_2) {
            wx = x - rpWidth;
            _drawParticle(wx, y);
        }

        if (y < ps_2) {
            wy = y + rpHeight;
            _drawParticle(x, wy);
        } else if (y > rpHeight - ps_2) {
            wy = y - rpHeight;
            _drawParticle(x, wy);
        }

        if (wx != x && wy != y)
            _drawParticle(wx, wy);
    }
}

void CellNoiseRenderer::_drawParticle(double x, double y)
{
    glPushMatrix();

    glTranslated(x, y, 0.0);
    glScaled(_particleSize, _particleSize, 1.0);

    // TODO use VBO
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2d(0.0, 0.0);
        glVertex2d(-0.5, -0.5);

        glTexCoord2d(1.0, 0.0);
        glVertex2d(0.5, -0.5);

        glTexCoord2d(0.0, 1.0);
        glVertex2d(-0.5, 0.5);

        glTexCoord2d(1.0, 1.0);
        glVertex2d(0.5, 0.5);
    glEnd();

    glPopMatrix();
}
