#ifndef LIGHT_H_
#define LIGHT_H_

#include "Visitor.h"
#include "Vec.h"
#include "Mat.h"

struct Light : public Node
{
    enum LightType { POINT, DIRECTIONAL };
    LightType type;

    Vec3 color;
    Vec3 direction;

    bool hasShadow;
    GLuint shadowTexture;
    Mat4x4 shadowMatrix;

    void accept(Visitor *visitor) { visitor->visitLight(this); }

    Light(const char *name_) :
        Node(name_),
        type(POINT), color(1., 1., 1.), direction(0., 0., 0.),
        hasShadow(false) {};
};

#endif /* LIGHT_H_ */
