#include <math.h>

#include <boost/foreach.hpp>

#include "utils.h"
#include "CellNoiseRenderer.h"

CellNoiseRenderer::CellNoiseRenderer(
        ParticleSystem *noiseParticles,
        double particleSize) :

        _noiseParticles(noiseParticles), _particleSize(particleSize) {

    _ps_2 = _particleSize / 2.0;
    _maxD = 1.0 - sqrt(2.0);

    std::vector<Program::ShaderSpec> shaders;
    shaders.push_back(Program::ShaderSpec("cellNoise.fp", GL_FRAGMENT_SHADER));

    _cellNoiseProgram.setShaders(shaders);
}

void CellNoiseRenderer::reload()
{
    _cellNoiseProgram.reload();
}

void CellNoiseRenderer::setNoiseParticles(ParticleSystem *noiseParticles)
{
    _noiseParticles = noiseParticles;
}

void CellNoiseRenderer::_render(RenderPass *renderPass) {
    int rpWidth = renderPass->getWidth();
    int rpHeight = renderPass->getHeight();

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

        if (x < _ps_2) {
            wx = x + rpWidth;
            _drawParticle(wx, y);
        } else if (x > rpWidth - _ps_2) {
            wx = x - rpWidth;
            _drawParticle(wx, y);
        }

        if (y < _ps_2) {
            wy = y + rpHeight;
            _drawParticle(x, wy);
        } else if (y > rpHeight - _ps_2) {
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
