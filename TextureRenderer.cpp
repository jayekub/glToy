#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include "utils.h"
#include "Vec.h"
#include "TextureRenderer.h"

TextureRenderer::TextureRenderer()
{
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_maxTextures);

    // make quad to texture
    glGenBuffers(1, &_vertexBuffer);
    glGenBuffers(1, &_texcoordBuffer);

    const Vec3 vertices[4] = {
        Vec3(-1., 1., 0.), Vec3(1., 1., 0.),
        Vec3(1., -1., 0.), Vec3(-1., -1., 0.)};

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vec3), vertices,
                 GL_STATIC_DRAW);

    const Vec2 texcoords[4] = {
        Vec2(0., 0.), Vec2(1., 0.), Vec2(1., 1.), Vec2(0., 1.)};

    glBindBuffer(GL_ARRAY_BUFFER, _texcoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vec2), texcoords,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // make default program to simply texture the quad
    _defProgram.addShader(
        new Program::Shader("shaders/standard.vs", GL_VERTEX_SHADER));
    _defProgram.addShader(
        new Program::Shader("shaders/standard.fs", GL_FRAGMENT_SHADER));
    _defProgram.link();

    _modelMat = Mat4::identity();
    _viewMat = Mat4::identity();
    _projMat = Mat4::identity();
}

TextureRenderer::TextureRenderer(GLuint texture)
{
    TextureRenderer();
    setTexture(texture);
}

TextureRenderer::TextureRenderer(const std::vector<GLuint> &textures)
{
    TextureRenderer();
    setTextures(textures);
}

TextureRenderer::~TextureRenderer()
{
    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteBuffers(1, &_texcoordBuffer);
}

void TextureRenderer::render()
{
    int vpWidth = _renderPass->getWidth();
    int vpHeight = _renderPass->getHeight();

    _viewMat = Mat4::translate(Vec3(vpWidth / 2., vpHeight / 2., 0.)) *
               Mat4::scale(Vec3(vpWidth / 2., vpHeight / 2., 1.));

    _projMat = Mat4::ortho(0, vpWidth, vpHeight, 0, 0, 1);

    GLuint vertexInLoc = 0, texcoordInLoc = 0;

    Program *program = _program ? _program : &_defProgram;

    program->use();
    program->setUniform("modelMat", _modelMat)
            .setUniform("viewMat", _viewMat)
            .setUniform("projMat", _projMat);

    vertexInLoc = program->attribute("vertexIn");
    texcoordInLoc = program->attribute("texcoordIn");

    glEnableVertexAttribArray(vertexInLoc);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glVertexAttribPointer(vertexInLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(texcoordInLoc);

    glBindBuffer(GL_ARRAY_BUFFER, _texcoordBuffer);
    glVertexAttribPointer(texcoordInLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    program->resetSamplers();

    int tex = 0;
    BOOST_FOREACH(GLuint texture, _textures) {
        std::string argName = (boost::format("tex%1%") % tex).str();

        // XXX two hash lookups, could be optimized
        if (program->hasUniform(argName)) {
            program->setSampler(argName, GL_TEXTURE_2D, texture);
        }

        if (++tex >= _maxTextures)
            break;
    }

    _renderPass->begin();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glDrawArrays(GL_QUADS, 0, 4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    program->resetSamplers();

    glDisableVertexAttribArray(vertexInLoc);
    glDisableVertexAttribArray(texcoordInLoc);
    glDisableClientState(GL_VERTEX_ARRAY);

    _renderPass->end();
}

TextureRenderer &TextureRenderer::setTextures(
    const std::vector<GLuint> &textures)
{
    _textures = textures;
    return *this;
}

TextureRenderer &TextureRenderer::setTexture(GLuint texture)
{
    std::vector<GLuint> tmp;

    tmp.push_back(texture);
    return setTextures(tmp);
}

TextureRenderer &TextureRenderer::addTexture(GLuint texture)
{
    _textures.push_back(texture);
    return *this;
}

TextureRenderer &TextureRenderer::setProgram(Program *program)
{
    _program = program;
    return *this;
}
