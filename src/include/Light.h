#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "ptr.h"
#include "Visitor.h"
#include "Vec.h"
#include "Mat.h"

DEF_SHARED_PTR(Light);

struct Light : public Node
{
    enum LightType { POINT, DIRECTIONAL };
    LightType type;

    Vec3 color;
    Vec3 direction;

    bool hasShadow;
    GLuint shadowTexture;
    Mat4x4 shadowMatrix;

    void accept(VisitorPtr &visitor) const {
        visitor->visitLight(LightConstPtr(this)); }

    static LightPtr create(const std::string &name_) {
        return LightPtr(new Light(name_)); }

private:
    Light(const std::string &name_) :
        Node(name_),
        type(POINT), color(1., 1., 1.), direction(0., 0., 0.),
        hasShadow(false) {};
};

#endif // _LIGHT_H_
