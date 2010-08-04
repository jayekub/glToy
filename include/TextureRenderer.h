#ifndef TEXTURERENDERER_H_
#define TEXTURERENDERER_H_

#include "glToy.h"
#include "RenderPass.h"
#include "Renderer.h"

class TextureRenderer : public Renderer
{
public:
    TextureRenderer(GLuint texture) : _texture(texture) {};

    void render(RenderPass *renderPass);

    void setTexture(GLuint texture) { _texture = texture; }

private:
    GLuint _texture;
};

#endif /* TEXTURERENDERER_H_ */
