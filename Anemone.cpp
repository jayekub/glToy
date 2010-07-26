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

    _numPts = (_numSegments + 1) * _numTentacles;
    _numLines = _numSegments * _numTentacles;

    _tentacles = new Vec3[_numPts];
//    _indices = new uint[2 * _numLines];


    glGenBuffers(1, &_vertexBuffer);
    glGenBuffers(1, &_indexBuffer);

    // allocate vertex buffer. will be mapped and filled in update()
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _numPts * sizeof(Vec3),
                 0, GL_DYNAMIC_DRAW);

    // allocate and then map index buffer
    glBindBuffer(GL_ARRAY_BUFFER, _indexBuffer);

    glBufferData(GL_ARRAY_BUFFER, 2 * _numLines * sizeof(uint),
                 0, GL_STATIC_DRAW);
    uint *indices = (uint *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

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

            if (s < _numSegments) {
                indices[2 * index] = index;
                indices[2 * index + 1] = index + _numTentacles;
            }
        }
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    _anemoneProgram.addShader(
           Program::ShaderSpec("shaders/anemone.fp", GL_FRAGMENT_SHADER));
    _anemoneProgram.reload();
}

Anemone::~Anemone()
{
    delete [] _tentacles;
    //delete [] _vertices;
//    delete [] _indices;

    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteBuffers(1, &_indexBuffer);
}

void Anemone::update(double dt)
{
    _time += dt;
    Vec3 timeOffset = Vec3(1., 1., 1.) * (_time / 2.);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    _vertices = (Vec3 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int i = 0; i < _numPts; ++i) {
        double offset = 0.5 * PerlinNoise::noise3(
                _tentacles[i].x + timeOffset.x,
                _tentacles[i].y + timeOffset.y,
                _tentacles[i].z + timeOffset.z);

        _vertices[i] = _tentacles[i] + offset * _currentDir;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

/*
    glBufferData(GL_ARRAY_BUFFER, _numPts * sizeof(Vec3),
                 _vertices, GL_DYNAMIC_DRAW);*/
}

void Anemone::render()
{
    glEnable(GL_LINE_SMOOTH);

    glPushMatrix();

    glRotatef(fmod(10. * _time, 360.), 0., 1., 0.);
    glScalef(1., 1., 1.);

    glColor3f(1., 1., 0.8);

    _anemoneProgram.use();

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    for (int s = 0; s < _numSegments; ++s) {
        glLineWidth((float) (_numSegments - s) * 5. / (float) _numSegments);

       // int offset = 2 * s * _numTentacles;
        glDrawElements(GL_LINES, 2 * _numTentacles, GL_UNSIGNED_INT,
                       BUFFER_OFFSET(s * 2 * _numTentacles * sizeof(uint)));
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glPopMatrix();
}
