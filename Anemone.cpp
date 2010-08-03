#include <stdio.h>

#include "RenderPass.h"
#include "Mat.h"
#include "Noise.h"
#include "Camera.h"
#include "Light.h"

#include "Anemone.h"

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

    _tentacles = new Vec3[_numPts];
    _posed = new Vec3[_numPts];

    glGenBuffers(1, &_vertexBuffer);
    //glGenBuffers(1, &_indexBuffer);
    glGenBuffers(1, &_p0Buffer);
    glGenBuffers(1, &_p1Buffer);

    // allocate vertex buffer. will be mapped and filled in update()
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * _numLines * sizeof(Vec3),
                 0, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _p0Buffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * _numLines * sizeof(Vec3),
                 0, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _p1Buffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * _numLines * sizeof(Vec3),
                 0, GL_DYNAMIC_DRAW);

/*
    // allocate and then map index buffer
    glBindBuffer(GL_ARRAY_BUFFER, _indexBuffer);

    glBufferData(GL_ARRAY_BUFFER, 2 * _numLines * sizeof(uint),
                 0, GL_STATIC_DRAW);
    uint *indices = (uint *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
*/

    // generate rest pose points and also fill index buffer
    Vec3 tentacleDirs[_numTentacles];
    for (int i = 0; i < _numTentacles; ++i) {
        tentacleDirs[i] =  Vec3::randVec(-1., 1.);
    }

    for (int t = 0; t < _numTentacles; ++t) {
        for (int s = 0; s < _numSegments + 1; ++s) {
            int index = s * _numTentacles + t;

            _tentacles[index] =
                    ((float) s / (float) _numSegments) * tentacleDirs[t] +
                    wiggle * Vec3::randVec(-1., 1.);
/*
            if (s < _numSegments) {
                indices[2 * index] = index;
                indices[2 * index + 1] = index + _numTentacles;
            }
*/
        }
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    _anemoneProgram.addShader(
           Program::ShaderSpec("shaders/anemone.vp", GL_VERTEX_SHADER));

//    _anemoneProgram.addShader(
//            Program::ShaderSpec("shaders/matrix_utils.h", GL_FRAGMENT_SHADER));
    _anemoneProgram.addShader(
           Program::ShaderSpec("shaders/anemone.fp", GL_FRAGMENT_SHADER));
    _anemoneProgram.reload();

    _p0Loc = _anemoneProgram.attribute("p0");
    _p1Loc = _anemoneProgram.attribute("p1");
    _lineWidthLoc = _anemoneProgram.uniform("lineWidth");
    _cameraPosLoc = _anemoneProgram.uniform("cameraPos");
    _viewportLoc = _anemoneProgram.uniform("viewport");
    _fragToWorldLoc = _anemoneProgram.uniform("fragToWorld");
    //_projectionLoc = _anemoneProgram.uniform("projection");

    _lightPosLoc = _anemoneProgram.uniform("lightPos");
    _shadowMapLoc = _anemoneProgram.uniform("shadowMap");
    _shadowMatrixLoc = _anemoneProgram.uniform("shadowMatrix");
}

Anemone::~Anemone()
{
    delete [] _tentacles;
    delete [] _posed;

    glDeleteBuffers(1, &_vertexBuffer);
//    glDeleteBuffers(1, &_indexBuffer);
    glDeleteBuffers(1, &_p0Buffer);
    glDeleteBuffers(1, &_p1Buffer);
}

void Anemone::update(double dt)
{
    // calculate new offsets
    _time += dt;
    Vec3 timeOffset = Vec3(1., 1., 1.) * (_time / 2.);

    //for (int i = 0; i < _numPts; ++i) {
    for (int s = 0; s < _numSegments + 1; ++s) {
        for (int t = 0; t < _numTentacles; ++t) {

        int i = s * _numTentacles + t;

        double currentOffset = 0.5 * PerlinNoise::noise3(
                _tentacles[i].x + timeOffset.x,
                _tentacles[i].y + timeOffset.y,
                _tentacles[i].z + timeOffset.z);

        Vec3 magnetVec = _magnetPosition - _posed[i];

        _posed[i] = _tentacles[i] + currentOffset * _currentDir +
                    (s / (float) _numSegments) * _magnetStrength * magnetVec;

        }
    }

    // update vbos
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    Vec3 *vertices = (Vec3 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int i = 0; i < _numLines; ++i) {
        vertices[2 * i] = _posed[i];
        vertices[2 * i + 1] = _posed[i + _numTentacles];
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, _p0Buffer);
    Vec3 *p0 = (Vec3 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int i = 0; i < _numLines; ++i) {
        p0[2 * i] = _posed[i];
        p0[2 * i + 1] = _posed[i];
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, _p1Buffer);
    Vec3 *p1 = (Vec3 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int i = 0; i < _numLines; ++i) {
        p1[2 * i] = _posed[i + _numTentacles];
        p1[2 * i + 1] = _posed[i + _numTentacles];
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Anemone::render(const Scene::State *state)
{
    glEnable(GL_LINE_SMOOTH);

    glPushMatrix();

    glRotatef(fmod(10. * _time, 360.), 0., 1., 0.);
    //glScalef(1., 1., 1.);

    glColor3f(1., 1., 0.8);

    Mat4x4 modelView, projection;

    // model space is world space (for now)
    glGetFloatv(GL_MODELVIEW_MATRIX, modelView.v);
    glGetFloatv(GL_PROJECTION_MATRIX, projection.v);

    static Mat4x4 remap(2., 0., 0., -1.,
                        0., 2., 0., -1.,
                        0., 0., 2., -1.,
                        0., 0., 0., 1.);
    // XXX wtf?
    Mat4x4 fragToWorld = (projection * modelView).inverse() * remap;

    _anemoneProgram.use();

   // glUniform3f(_cameraPosLoc, 0., 0., -10.);

    glUniform3f(_cameraPosLoc,
                state->camera->position.x,
                state->camera->position.y,
                state->camera->position.z);

    glUniform4f(_viewportLoc, 0., 0.,
            1. / (float) state->renderPass->getWidth(),
            1. / (float) state->renderPass->getHeight());

    glUniformMatrix4fv(_fragToWorldLoc, 1, GL_FALSE, fragToWorld.v);
    //glUniformMatrix4fv(_projectionLoc, 1, GL_FALSE, projection.v);


    // XXX fragile
    const Light *firstLight = state->lights.begin()->second;
    const Vec3 &firstLightPos = state->lightPositions.begin()->second;

    glUniform3f(_lightPosLoc,
                firstLightPos.x, firstLightPos.y, firstLightPos.z);

    glUniformMatrix4fv(_shadowMatrixLoc, 1, GL_FALSE,
                       firstLight->shadowMatrix.v);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, firstLight->shadowTexture);

    glUniform1i(_shadowMapLoc, 1);

    glEnableClientState(GL_VERTEX_ARRAY);

    glEnableVertexAttribArray(_p0Loc);
    glEnableVertexAttribArray(_p1Loc);

    glBindBuffer(GL_ARRAY_BUFFER, _p0Buffer);
    glVertexAttribPointer(_p0Loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glBindBuffer(GL_ARRAY_BUFFER, _p1Buffer);
    glVertexAttribPointer(_p1Loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

    glVertexPointer(3, GL_FLOAT, 0, 0);

    for (int s = 0; s < _numSegments; ++s) {
        float segFrac =
                (float) (_numSegments - s) / (float) _numSegments;
        float lineWidthHint = pow(segFrac, 1.) * _maxWidth;
        glLineWidth(lineWidthHint);

        float lineWidth;
        glGetFloatv(GL_LINE_WIDTH, &lineWidth);
        glUniform1f(_lineWidthLoc, lineWidth / 50.);

        glDrawArrays(GL_LINES, s * 2 * _numTentacles, 2 * _numTentacles);
    }

    //glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(_p0Loc);
    glDisableVertexAttribArray(_p1Loc);

    glDisableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glPopMatrix();
}
