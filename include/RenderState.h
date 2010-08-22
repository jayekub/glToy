#ifndef RENDERSTATE_H_
#define RENDERSTATE_H_

#include <stack>
#include <string>
#include <unordered_map>

#include "Vec.h"
#include "Mat.h"

class RenderPass;
class Camera;
class Light;

struct RenderState
{
    RenderPass *renderPass;

    std::string cameraName;
    Camera *camera;

    Mat4 viewMat;
    Mat4 projectionMat;

    std::unordered_map<const char *, Light *> lights;
    std::unordered_map<const char *, Vec3> lightPositions;

    void reset()
    {
        renderPass = NULL;

        cameraName = "";
        camera = NULL;

        viewMat = Mat4::identity();
        projectionMat = Mat4::identity();

        lights.clear();
        lightPositions.clear();

        // why doesn't std::stack implement clear()?
        while (!_transformStack.empty())
            _transformStack.pop();

        _transformStack.push(Mat4::identity());
    };

    void pushTransformMat() { _transformStack.push(_transformStack.top()); }
    void popTransformMat() { _transformStack.pop(); }
    void multTransformMat(const Mat4 &mat) { _transformStack.top() *= mat; }

    const Mat4 &getTransformMat() const { return _transformStack.top(); }

private:
    std::stack<Mat4> _transformStack;
};

#endif /* RENDERSTATE_H_ */
