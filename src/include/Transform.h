#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <string>

#include "ptr.h"
#include "Node.h"
#include "Visitor.h"
#include "Mat.h"

DEF_SHARED_PTR(Transform);

struct Transform : public Node
{
    Mat4 matrix;

    virtual void accept(VisitorPtr &visitor) const {
        visitor->visitTransform(TransformConstPtr(this)); }

    static TransformPtr create(const std::string &name_) {
        return TransformPtr(new Transform(name_)); }

private:
    Transform(const std::string &name_) :
        Node(name_), matrix(Mat4::identity()) { }
};

#endif // _TRANSFORM_H_
