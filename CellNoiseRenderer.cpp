#include <math.h>

#include <boost/foreach.hpp>

#include "utils.h"
#include "CellNoiseRenderer.h"

CellNoiseRenderer::CellNoiseRenderer(
        ParticleSystem *noiseParticles,
        double particleSize) :

        _noiseParticles(noiseParticles), _particleSize(particleSize),
        _particleDisplayList(0) {

    std::vector<Program::ShaderSpec> shaders;
    shaders.push_back(Program::ShaderSpec("cellNoise.fp", GL_FRAGMENT_SHADER));

    _cellNoiseProgram.setShaders(shaders);
    reload();
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

    //if (glIsList(_particleDisplayList)) {
    //    glDeleteLists(_particleDisplayList, 1);
    //}

    _particleDisplayList = 1;

    glNewList(_particleDisplayList, GL_COMPILE);

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

    glEndList();
}

void CellNoiseRenderer::_render(RenderPass *renderPass) {
    int rpWidth = renderPass->getWidth();
    int rpHeight = renderPass->getHeight();

    double ps = _particleSize * rpWidth;
    double ps_2 = ps / 2.0;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _cellNoiseProgram.use();

    BOOST_FOREACH(Particle *p, _noiseParticles->getParticles()) {
        double x = p->position.x * rpWidth;
        double y = p->position.y * rpHeight;

        _drawParticle(x, y, ps);

        // test if we need to draw a wrapped version of this particle also
        // to make the noise tileable
        double wx = x, wy = y;

        if (x < ps_2) {
            wx = x + rpWidth;
            _drawParticle(wx, y, ps);
        } else if (x > rpWidth - ps_2) {
            wx = x - rpWidth;
            _drawParticle(wx, y, ps);
        }

        if (y < ps_2) {
            wy = y + rpHeight;
            _drawParticle(x, wy, ps);
        } else if (y > rpHeight - ps_2) {
            wy = y - rpHeight;
            _drawParticle(x, wy, ps);
        }

        if (wx != x && wy != y)
            _drawParticle(wx, wy, ps);
    }
}

void CellNoiseRenderer::_drawParticle(double x, double y, double size)
{
    glPushMatrix();

    glTranslated(x, y, 0.0);
    glScaled(size, size, 1.0);

    // TODO use VBO
    glCallList(_particleDisplayList);

/*
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
*/
    glPopMatrix();
}
