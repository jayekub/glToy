#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Node.h"
#include "Visitor.h"
#include "Vec.h"

struct Transform : public Node
{
    Vec3 translation;

    float rotationAngle;
    Vec3 rotationAxis;

    Vec3 scale;

    virtual void accept(Visitor *visitor) { visitor->visitTransform(this); }

    Transform(const char *name) :
        Node(name),
        translation(0., 0., 0.), rotationAngle(0.), rotationAxis(0., 1., 0.),
        scale(1., 1., 1.) {};
};

#endif /* TRANSFORM_H_ */
