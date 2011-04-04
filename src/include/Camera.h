#ifndef CAMERA_H_
#define CAMERA_H_

#include "Node.h"
#include "Visitor.h"
#include "Vec.h"

class Light;

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
    Light *light;

    // motion blur params
    double dt;

    void accept(Visitor *visitor) { visitor->visitCamera(this); }

    Camera(const char *name_) :
        Node(name_),
        projection(PERSP), nearClip(1.), farClip(1000.),
        position(0., 0., 0.), center(0., 0., 1.), up(0., 1., 0.),
        width(2.), height(2.), fov(45),
        isShadowCamera(false), light(NULL), dt(0.) {};
};

#endif /* CAMERA_H_ */
