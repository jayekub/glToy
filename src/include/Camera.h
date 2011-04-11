#ifndef CAMERA_H_
#define CAMERA_H_

#include <string>

#include "ptr.h"
#include "Node.h"
#include "Light.h"
#include "Visitor.h"
#include "Vec.h"

DEF_SHARED_PTR(Camera);

struct Camera : public Node
{
    enum ProjectionType { FLAT, ORTHO, PERSP };
    ProjectionType projection;

    // common params
    float nearClip, farClip;

    Vec3 position, center, up;

    // ortho params
    float width, height;

    // persp params
    float fov;

    // shadow cam params
    bool isShadowCamera;
    LightPtr light;

    // motion blur params
    double dt;

    void accept(VisitorPtr &visitor) const {
        visitor->visitCamera(CameraConstPtr(this)); }

    DEF_CREATE_1(Camera, const std::string &, name_);

private
    Camera(const std::string &name_) :
        Node(name_),
        projection(PERSP), nearClip(1.), farClip(1000.),
        position(0., 0., 0.), center(0., 0., 1.), up(0., 1., 0.),
        width(2.), height(2.), fov(45),
        isShadowCamera(false), light(NULL), dt(0.) {};
};

#endif /* CAMERA_H_ */
