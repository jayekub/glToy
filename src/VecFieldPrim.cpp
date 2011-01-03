#include "VecField.h"

#include "VecFieldPrim.h"

VecFieldPrim::VecFieldPrim(const char *name, const VecField *vecField) :
    Prim(name), _vecField(vecField), _color(1., 1., 1.)
{
    const Vec3 &size = _vecField->getSize();
    const size_t numVecs = (size_t) size.x*size.y*size.z;

    glGenBuffers(1, &_vecBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, _vecBuffer);

    glBufferData(GL_ARRAY_BUFFER, 2.*numVecs*sizeof(Vec3), 0,
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ////

    _vecProgram.addShader(
        new Program::Shader("shaders/standard.vs", GL_VERTEX_SHADER));

    _vecProgram.addShader(
        new Program::Shader("shaders/constant.fs", GL_FRAGMENT_SHADER));

    _vecProgram.link();
}

VecFieldPrim::~VecFieldPrim()
{
    glDeleteBuffers(1, &_vecBuffer);
}

void VecFieldPrim::render(RenderState &state)
{
    const Vec3 &size = _vecField->getSize();
    const size_t numVecs = (size_t) size.x*size.y*size.z;

    //fprintf(stderr, "VecFieldPrim::render size %s numVecs %d\n",
    //        size.toString().c_str(), (int) numVecs);

    // put lines representing vectors into _vecBuffer
    glBindBuffer(GL_ARRAY_BUFFER, _vecBuffer);

    Vec3 *vecs = (Vec3 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int z = 0; z < size.z; ++z)
        for (int y = 0; y < size.y; ++y)
            for (int x = 0; x < size.x; ++x) {
                const Vec3 p(x, y, z);
                const Vec3 &v = (*_vecField)(x, y, z);
                const int idx = (z*size.x*size.y + y*size.x + x);

                vecs[2*idx] = p;
                vecs[2*idx + 1] = p + v;

                //fprintf(stderr, "v = %s at %s\n", v.toString().c_str(),
                //        p.toString().c_str());
            }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    _vecProgram.use();
    _vecProgram.setUniform("modelMat", state.getTransformMat())
               .setUniform("viewMat", state.viewMat)
               .setUniform("projMat", state.projectionMat)
               .setUniform("color", _color);

    const GLuint vertexInLoc = _vecProgram.attribute("vertexIn");

    glEnableVertexAttribArray(vertexInLoc);

    glVertexAttribPointer(vertexInLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINES, 0, 2.*numVecs);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glDisableVertexAttribArray(vertexInLoc);
}
