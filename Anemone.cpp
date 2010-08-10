#include <stdio.h>

#include "RenderPass.h"
#include "Mat.h"
#include "Noise.h"
#include "Camera.h"
#include "Light.h"

#include "Anemone.h"

#define T_IX(t, s) (t * _numSegments + s)
#define TUBE_SIDES 10.

Anemone::Anemone(
        const char *name,
        int numTentacles, int numSegments, int maxWidth, double wiggle) :
    Prim(name),
    _numTentacles(numTentacles), _numSegments(numSegments),
    _maxWidth(maxWidth), _time(0.0), _magnetStrength(0.)
{
    _currentDir = Vec3::randVec(-1., 1.);

    _numPts = (_numSegments + 1) * _numTentacles;
    _numLines = _numSegments * _numTentacles;

    glGenBuffers(1, &_vertexBuffer);
    glGenBuffers(1, &_pointNumBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _numTentacles * sizeof(Vec3), 0,
                 GL_STATIC_DRAW);

    Vec3 *vertices = (Vec3 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int i = 0; i < _numTentacles; ++i)
        vertices[i] = 5. * Vec3::randVec(-1., 1.); //Vec3(0., 0., 0.);

    glUnmapBuffer(GL_ARRAY_BUFFER);

    /*
    glBindBuffer(GL_ARRAY_BUFFER, _pointNumBuffer);
    glBufferData(GL_ARRAY_BUFFER, _numTentacles, 0, GL_STATIC_DRAW);

    int *pointNums = (Vec3 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int i = 0; i < _numTentacles; ++i)
        pointNums[i] = i;

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);*/

    _anemoneProgram.addShader(
           new Program::Shader("shaders/anemone.vs", GL_VERTEX_SHADER));

    _anemoneProgram.addShader(
           &((new Program::Shader(GL_GEOMETRY_SHADER))
                ->addFile("shaders/common.inc")
                 .addFile("shaders/tube.inc")
                 .addFile("shaders/noise.inc")
                 .addFile("shaders/anemone.gs").compile()));

    _anemoneProgram.addShader(
           &((new Program::Shader(GL_FRAGMENT_SHADER))
                   ->addFile("shaders/tube.inc")
                    .addFile("shaders/anemone.fs").compile()));

    _anemoneProgram.link();

    _permTexture = GLSLNoise::makePermutationTexture();
    _gradTexture = GLSLNoise::makeGradientTexture();
}

Anemone::~Anemone()
{
    glDeleteBuffers(1, &_vertexBuffer);
}

void Anemone::update(double dt)
{
    // calculate new offsets
    _time += dt;

    /*
    Vec3 timeOffset = Vec3(1., 1., 1.) * (_time / 2.);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    Vec3 *vertices = (Vec3 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int t = 0; t < _numTentacles; ++t) {
        for (int s = 0; s < _numSegments + 1; ++s) {
            int i = T_IX(t, s);

            double currentOffset = 0.5 * PerlinNoise::noise3(
                _restPoints[i].x + timeOffset.x,
                _restPoints[i].y + timeOffset.y,
                _restPoints[i].z + timeOffset.z);

            //Vec3 magnetVec = _magnetPosition - _posed[i];

            vertices[i] = _restPoints[i] + currentOffset * _currentDir;
                //(s / (float) _numSegments) * _magnetStrength * magnetVec;
        }
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}

void Anemone::render(const Scene::State *state)
{
    glPushMatrix();

    _anemoneProgram.use();
    //_anemoneProgram.setDebug(true);

    Mat4x4 modelView, projection;

    // model space is world space (for now)
    glGetFloatv(GL_MODELVIEW_MATRIX, modelView.v);
    glGetFloatv(GL_PROJECTION_MATRIX, projection.v);

    _anemoneProgram.setUniform("modelViewMat", modelView);
    _anemoneProgram.setUniform("projMat", projection);

    _anemoneProgram.setUniform("time", (float) _time);
    _anemoneProgram.setUniform("tubeNumSegments", _numSegments);
    _anemoneProgram.setUniform("tubeNumSides", 6);

    _anemoneProgram.setUniform("cameraPos", state->camera->position);

    // XXX fragile
    const Light *firstLight = state->lights.begin()->second;
    const Vec3 &firstLightPos = state->lightPositions.begin()->second;

    _anemoneProgram.setUniform("lightPos", firstLightPos);
    _anemoneProgram.setUniform("shadowMatrix", firstLight->shadowMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, firstLight->shadowTexture);

    _anemoneProgram.setUniform("shadowMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _permTexture);

    _anemoneProgram.setUniform("permTexture", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _gradTexture);

    _anemoneProgram.setUniform("gradTexture", 2);

    glEnableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glPointSize(5.);
    glColor3f(1., 0., 0.);

    glDrawArrays(GL_POINTS, 0, _numTentacles);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPopMatrix();
}
