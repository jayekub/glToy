#ifndef RENDERSTATE_H_
#define RENDERSTATE_H_

#include <stack>
#include <string>
#include <unordered_map>

#include "RenderPass.h"
#include "Camera.h"
#include "Light.h"
#include "Vec.h"
#include "Mat.h"

struct RenderState
{
    RenderPassConstPtr renderPass;

    std::string cameraName;
    CameraConstPtr camera;

    Mat4 viewMat;
    Mat4 projectionMat;

    std::unordered_map<std::string, LightConstPtr> lights;
    std::unordered_map<std::string, Vec3> lightPositions;

    void reset()
    {
        renderPass = RenderPassPtr();

        cameraName = "";
        camera = CameraPtr();

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
