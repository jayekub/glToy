#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include "utils.h"
#include "TextureRenderer.h"

TextureRenderer::TextureRenderer()
{
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_maxTextures);
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

void TextureRenderer::render()
{
    int vpWidth = _renderPass->getWidth();
    int vpHeight = _renderPass->getHeight();

    if (_program)
        _program->use();
    else
        glUseProgram(0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0.0, 0.0, vpWidth, vpHeight);
    glOrtho(0, vpWidth, vpHeight, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0, 1.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glEnable(GL_TEXTURE_2D);

    int tex = 0;
    BOOST_FOREACH(GLuint texture, _textures) {
        glActiveTexture(GL_TEXTURE0 + tex);
        glBindTexture(GL_TEXTURE_2D, texture);

        //printf("binding texture %d to unit %d\n", (int) texture, tex);

        if (_program) {
            std::string argName = (boost::format("tex%1%") % tex).str();

            if (_program->hasUniform(argName)) {
                //printf("attaching unit %d to uniform %s\n", tex, argName.c_str());
                glUniform1i(_program->uniform(argName), tex);
            }
        }

        if (++tex >= _maxTextures)
            break;
    }

    //PRINT_GLERROR("tr1");

    drawViewportQuad(vpWidth, vpHeight);

    //glDisable(GL_TEXTURE_2D);
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

TextureRenderer &TextureRenderer::setProgram(Program *program)
{
    _program = program;
    return *this;
}
