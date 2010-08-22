#include "glToy.h"
#include "Camera.h"
#include "Bubble.h"

Bubble::Bubble(const char *name, float radius, int numLat, int numLong) :
    Prim(name), _radius(radius), _numLat(numLat), _numLong(numLong)
{
    _bubbleProgram.addShader(
        new Program::Shader("shaders/bubble.vs", GL_VERTEX_SHADER));

    _bubbleProgram.addShader(
        new Program::Shader("shaders/bubble.gs", GL_GEOMETRY_SHADER));

    _bubbleProgram.addShader(
        &((new Program::Shader(GL_FRAGMENT_SHADER))
        ->addFile("shaders/sphere.inc")
        .addFile("shaders/bubble.fs").compile()));

    _bubbleProgram.link();
}

Bubble::~Bubble()
{
}

void Bubble::update(double dt)
{
}


void Bubble::render(const RenderState &state)
{
    _bubbleProgram.use();

    _bubbleProgram.setUniform("modelMat", state.getTransformMat());
    _bubbleProgram.setUniform("viewMat", state.viewMat);
    _bubbleProgram.setUniform("projMat", state.projectionMat);

    _bubbleProgram.setUniform("cameraPos", state.camera->position);

    _bubbleProgram.setUniform("numLat", _numLat);
    _bubbleProgram.setUniform("numLong", _numLong);
    _bubbleProgram.setUniform("radius", _radius);

    glBegin(GL_POINTS);
    glVertex3f(0.f, 0.f, 0.f);
    glEnd();
}

