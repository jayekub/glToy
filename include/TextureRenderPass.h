#ifndef TEXTURERENDERPASS_H_
#define TEXTURERENDERPASS_H_

#include "glToy.h"

#include "RenderPass.h"

class TextureRenderPass : public RenderPass
{
public:
    TextureRenderPass(int width, int height);
    virtual ~TextureRenderPass();

    virtual void begin();
    virtual void end();

    virtual void setSize(int width, int height);

    GLuint getTexture() { return _texture; }
    GLuint getDepthTexture() { return _depthTexture; }

protected:
    GLuint _frameBuffer, _depthBuffer, _texture, _depthTexture;

    // For use by subclasses
    TextureRenderPass() {};
};

#endif /* TEXTURERENDERPASS_H_ */
