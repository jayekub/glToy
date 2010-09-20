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

    // common options
    float nearClip, farClip;

    Vec3 position, center, up;

    // ortho options
    float width, height;

    // persp options
    float fov;

    // shadow cam options
    bool isShadowCamera;
    Light *light;

    void accept(Visitor *visitor) { visitor->visitCamera(this); }

    Camera(const char *name_) :
        Node(name_),
        projection(PERSP), nearClip(1.), farClip(1000.),
        position(0., 0., 0.), center(0., 0., 1.), up(0., 1., 0.),
        width(2.), height(2.), fov(45),
        isShadowCamera(false), light(NULL) {};
};

#endif /* CAMERA_H_ */
