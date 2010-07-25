#include <math.h>

#include <boost/foreach.hpp>

#include "utils.h"
#include "CellNoiseRenderer.h"

CellNoiseRenderer::CellNoiseRenderer()
{
    _cellNoiseProgram.addShader(
            Program::ShaderSpec("shaders/cellNoise.fp", GL_FRAGMENT_SHADER));
    reload();
}

void CellNoiseRenderer::render(
        RenderPass *renderPass,
        ParticleSystem *noiseParticles,
        float particleSize)
{
    renderPass->setFlatProjection();
    _cellNoiseProgram.use();

    int rpWidth = renderPass->getWidth();
    int rpHeight = renderPass->getHeight();

    float ps = particleSize * rpWidth;
    float ps_2 = ps / 2.0;

    BOOST_FOREACH(Particle *p, noiseParticles->getParticles()) {
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

void CellNoiseRenderer::reload()
{
    _particleDisplayList = 1;

    glNewList(_particleDisplayList, GL_COMPILE);

        glBegin(GL_TRIANGLE_STRIP);

            glTexCoord2f(0.0, 0.0);
            glVertex2f(-0.5, -0.5);

            glTexCoord2f(1.0, 0.0);
            glVertex2f(0.5, -0.5);

            glTexCoord2f(0.0, 1.0);
            glVertex2f(-0.5, 0.5);

            glTexCoord2f(1.0, 1.0);
            glVertex2f(0.5, 0.5);

        glEnd();

    glEndList();
}

void CellNoiseRenderer::_drawParticle(float x, float y, float size)
{
    glPushMatrix();

    glTranslatef(x, y, 0.0);
    glScalef(size, size, 1.0);

    // TODO use VBO?
    glCallList(_particleDisplayList);

    glPopMatrix();
}
