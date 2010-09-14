#ifndef TEXTURERENDERPASS_H_
#define TEXTURERENDERPASS_H_

#include "glToy.h"

#include "RenderPass.h"

// TODO allow use of depth texture to be configurable
// TODO allow multiple render targets
class TextureRenderPass : public RenderPass
{
public:
    TextureRenderPass(int width, int height, float scale = 1.);
    virtual ~TextureRenderPass();

    virtual void begin();
    virtual void end();

    virtual void setSize(int width, int height);
    virtual void setScale(float scale) { _scale = scale; }

    GLuint getTexture() { return _texture; }
    GLuint getDepthTexture() { return _depthTexture; }

protected:
    float _scale;
    GLuint _frameBuffer, _depthBuffer, _texture, _depthTexture;

    // For use by subclasses
    TextureRenderPass() {};
};

#endif /* TEXTURERENDERPASS_H_ */
