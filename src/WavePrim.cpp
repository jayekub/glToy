#include "glToy.h"
#include "Noise.h"
#include "Camera.h"

#include "WavePrim.h"

WavePrim::WavePrim(const char *name, int size) :
    Prim(name), _size(size), _time(0.)
{
    glGenBuffers(1, &_vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _size*_size*sizeof(Vec3), 0, GL_STATIC_DRAW);

    Vec3 *vertices = (Vec3 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int x = 0; x < _size; ++x)
        for (int y = 0; y < _size; ++y)
            vertices[y*_size + x] = Vec3(x, 0, y);

    glUnmapBuffer(GL_ARRAY_BUFFER);

////

    //_waveProgram.setDebug(true);

    _waveProgram.addShader(
        (new Program::Shader(GL_VERTEX_SHADER))
             ->addFile("shaders/standard.vs").compile());

    _waveProgram.addShader(
        (new Program::Shader(GL_GEOMETRY_SHADER))
            ->addFile("shaders/common.inc")
             .addFile("shaders/wave.gs").compile());

    _waveProgram.addShader(
        ((new Program::Shader(GL_FRAGMENT_SHADER))
        ->addFile("shaders/common.inc")
         .addFile("shaders/wave.fs").compile()));

    _waveProgram.link();
}

WavePrim::~WavePrim()
{
    glDeleteBuffers(1, &_vertexBuffer);
}

void WavePrim::update(double dt)
{
    _time += dt;
}

void WavePrim::render(RenderState &state)
{
    _waveProgram.use();
    _waveProgram.setUniform("modelMat", state.getTransformMat())
                .setUniform("viewMat", state.viewMat)
                .setUniform("projMat", state.projectionMat)
                .setUniform("cameraPos", state.camera->position)
                .setUniform("time", (float) _time)
                .setUniform("color", Vec3(0, 0, 1));

    const GLuint vertexInLoc = _waveProgram.attribute("vertexIn");

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

    glEnableVertexAttribArray(vertexInLoc);

    glVertexAttribPointer(vertexInLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);

    //glPointSize(5.);
    glDrawArrays(GL_POINTS, 0, _size*_size);

    glDisableVertexAttribArray(vertexInLoc);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
