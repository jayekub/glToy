#ifndef CAMERA_H_
#define CAMERA_H_

#include "Node.h"
#include "Visitor.h"
#include "Vec.h"

struct Camera : public Node
{
    enum ProjectionType { FLAT, PERSP };
    ProjectionType projection;

    float fov;
    float nearClip;
    float farClip;

    Vec3 position, center, up;

    void accept(Visitor *visitor) { visitor->visitCamera(this); }

    Camera(const char *name_) :
        Node(name_),
        projection(PERSP), fov(45.), nearClip(1.), farClip(1000.),
        position(0., 0., 0.), center(0., 0., 1.), up(0., 1., 0.) {};
};

#endif /* CAMERA_H_ */
