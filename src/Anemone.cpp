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
        vertices[i] = Vec3(0., 0., 0.);

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
           new Program::Shader("shaders/standard.vs", GL_VERTEX_SHADER));

    _anemoneProgram.addShader(
           &((new Program::Shader(GL_GEOMETRY_SHADER))
                ->addFile("shaders/common.inc")
                 .addFile("shaders/tube.inc")
                 .addFile("shaders/noise.inc")
                 .addFile("shaders/anemone.gs").compile()));

    _anemoneProgram.addShader(
           &((new Program::Shader(GL_FRAGMENT_SHADER))
                   ->addFile("shaders/common.inc")
                    .addFile("shaders/tube.inc")
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
        }at
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}

void Anemone::render(RenderState &state)
{
    _anemoneProgram.use();
    //_anemoneProgram.setDebug(true);

    _anemoneProgram.setUniform("modelMat", state.getTransformMat());
    _anemoneProgram.setUniform("viewMat", state.viewMat);
    _anemoneProgram.setUniform("projMat", state.projectionMat);

//    fprintf(stderr, "model: %s\nview: %s\nproj: %s\nmvp: %s\n\n",
//            state.getTransformMat().toString().c_str(),
//            state.viewMat.toString().c_str(),
//            state.projectionMat.toString().c_str(),
//            (state.getTransformMat() * state.viewMat * state.projectionMat).toString().c_str());

    _anemoneProgram.setUniform("time", (float) _time);
    _anemoneProgram.setUniform("tubeNumSegments", _numSegments);
    _anemoneProgram.setUniform("tubeNumSides", 20); // XXX fix to work with odd

    _anemoneProgram.setUniform("cameraPos", state.camera->position);

    // XXX fragile
    const Light *firstLight = state.lights.begin()->second;
    const Vec3 &firstLightPos = state.lightPositions.begin()->second;

    _anemoneProgram.setUniform("lightPos", firstLightPos);
    _anemoneProgram.setUniform("shadowMatrix", firstLight->shadowMatrix);

    _anemoneProgram.resetSamplers();

    _anemoneProgram.setSampler("permTexture", GL_TEXTURE_2D,
                               _permTexture);

    _anemoneProgram.setSampler("gradTexture", GL_TEXTURE_2D,
                               _gradTexture);

    _anemoneProgram.setSampler("shadowMap", GL_TEXTURE_2D,
                               firstLight->shadowTexture);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

    glPointSize(5.);
    glDrawArrays(GL_POINTS, 0, _numTentacles);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
