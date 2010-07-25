#include <stdio.h>

#include "Noise.h"
#include "Anemone.h"

Anemone::Anemone(
        const char *name,
        int numTentacles, int numSegments, double wiggle) :
    Prim(name),
    _numTentacles(numTentacles), _numSegments(numSegments), _time(0.0)
{
    _currentDir = Vec3::randVec(-1., 1.);

    _tentacles = new Vec3*[_numTentacles];
    _currentOffsets = new double*[_numTentacles];

    for (int t = 0; t < _numTentacles; ++t) {
        _tentacles[t] = new Vec3[_numSegments];
        _currentOffsets[t] = new double[_numSegments];

        Vec3 dir = Vec3::randVec(-1., 1.);

        for (int s = 0; s < _numSegments; ++s) {
            _tentacles[t][s] = ((float) s / (float) _numSegments) * dir +
                               wiggle * Vec3::randVec(-1., 1.);
            _currentOffsets[t][s] = 0.0;
        }
    }

   _anemoneProgram.addShader(
           Program::ShaderSpec("shaders/anemone.fp", GL_FRAGMENT_SHADER));
    _anemoneProgram.reload();
}

Anemone::~Anemone()
{
    for (int t = 0; t < _numTentacles; ++t) {
        delete [] _tentacles[t];
    }

    delete [] _tentacles;
}

void Anemone::update(double dt)
{
    _time += dt;
    Vec3 offset = Vec3(1., 1., 1.) * (_time / 2.);

    for (int t = 0; t < _numTentacles; ++t) {
        for (int s = 0; s < _numSegments; ++s) {
            _currentOffsets[t][s] = 0.5 * PerlinNoise::noise3(
                _tentacles[t][s].x + offset.x,
                _tentacles[t][s].y + offset.y,
                _tentacles[t][s].z + offset.z);
        }
    }
}

void Anemone::render()
{
    glEnable(GL_LINE_SMOOTH);

    glPushMatrix();

    glRotatef(fmod(10. * _time, 360.), 0., 1., 0.);
    glScalef(5., 5., 5.);

    glColor3f(1., 1., 0.8);

    _anemoneProgram.use();

    for (int t = 0; t < _numTentacles; ++t) {
        //glBegin(GL_LINE_STRIP);

        for (int s = 0; s < _numSegments - 1; ++s) {
            glLineWidth((float) (_numSegments - s) * 5. / (float) _numSegments);

            Vec3 p0 = _tentacles[t][s] + _currentOffsets[t][s] * _currentDir,
                 p1 = _tentacles[t][s + 1] + _currentOffsets[t][s + 1] * _currentDir;

            glBegin(GL_LINES);
            glVertex3f(p0.x, p0.y, p0.z);
            glVertex3f(p1.x, p1.y, p1.z);
            glEnd();
        }

        //glEnd();
    }

    glPopMatrix();
}
