#include "glToy.h"
#include "Noise.h"
#include "Camera.h"
#include "Bubble.h"

Bubble::Bubble(const char *name, float radius,
               int numBubbles, int numLat, int numLong) :
    Prim(name), _radius(radius),
    _numBubbles(numBubbles), _numLat(numLat), _numLong(numLong)
{
    glGenBuffers(1, &_vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _numBubbles * sizeof(Vec3), 0,
                 GL_STATIC_DRAW);

    Vec3 *vertices = (Vec3 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int i = 0; i < _numBubbles; ++i)
        vertices[i] = Vec3(0., 0., 0.);

    glUnmapBuffer(GL_ARRAY_BUFFER);

    _bubbleProgram.addShader(
        new Program::Shader("shaders/vertexid.vs", GL_VERTEX_SHADER));

    _bubbleProgram.addShader(
        (new Program::Shader(GL_GEOMETRY_SHADER))
            ->addFile("shaders/common.inc")
             //.addFile("shaders/sphere.inc")
             //.addFile("shaders/noise.inc")
             .addFile("shaders/bubble.gs").compile());

    _bubbleProgram.addShader(
        ((new Program::Shader(GL_FRAGMENT_SHADER))
        ->addFile("shaders/common.inc")
         .addFile("shaders/sphere.inc")
         .addFile("shaders/bubble.fs").compile()));

    _bubbleProgram.link();

    _permTexture = GLSLNoise::makePermutationTexture();
    _gradTexture = GLSLNoise::makeGradientTexture();
}

Bubble::~Bubble()
{
    glDeleteBuffers(1, &_vertexBuffer);
}

void Bubble::update(double dt)
{
}


void Bubble::render(const RenderState &state)
{

    // XXX fragile
    //const Light *firstLight = state.lights.begin()->second;
    const Vec3 &firstLightPos = state.lightPositions.begin()->second;

    _bubbleProgram.use();
    _bubbleProgram.setUniform("modelMat", state.getTransformMat())
                  .setUniform("viewMat", state.viewMat)
                  .setUniform("projMat", state.projectionMat)
                  .setUniform("cameraPos", state.camera->position)
                  .setUniform("lightPos", firstLightPos)
                  .setUniform("numLat", _numLat)
                  .setUniform("numLong", _numLong)
                  .setUniform("radius", _radius)
                  .setUniform("numBubbles", _numBubbles)
                  .resetSamplers()
                  .setSampler("permTexture", GL_TEXTURE_2D, _permTexture)
                  .setSampler("gradTexture", GL_TEXTURE_2D, _gradTexture);

    glEnableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_POINTS, 0, _numBubbles);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

