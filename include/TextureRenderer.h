#ifndef TEXTURERENDERER_H_
#define TEXTURERENDERER_H_

#include <vector>

#include "glToy.h"
#include "RenderPass.h"
#include "Renderer.h"
#include "Program.h"

class TextureRenderer : public Renderer
{
public:
    TextureRenderer();
    TextureRenderer(GLuint texture);
    TextureRenderer(const std::vector<GLuint> &textures);

    virtual void render();

    TextureRenderer &setTextures(const std::vector<GLuint> &textures);
    TextureRenderer &setTexture(GLuint texture);

    TextureRenderer &setProgram(Program *program);

private:
    std::vector<GLuint> _textures;
    Program *_program;

    int _maxTextures;
};

#endif /* TEXTURERENDERER_H_ */
