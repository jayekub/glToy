#ifndef RENDERPASS_H_
#define RENDERPASS_H_

class RenderPass
{
public:
    RenderPass(int width, int height);
    virtual ~RenderPass();

    virtual void begin() = 0;
    virtual void end() = 0;

    virtual void setSize(int width, int height);

    int getWidth() { return _width; }
    int getHeight() { return _height; }

protected:
    int _width, _height;

    RenderPass() {};
};

#endif /* RENDERPASS_H_ */
