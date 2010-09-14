#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Node.h"
#include "Visitor.h"
#include "Mat.h"

struct Transform : public Node
{
    Mat4 matrix;

    virtual void accept(Visitor *visitor) { visitor->visitTransform(this); }

    Transform(const char *name) :
        Node(name), matrix(Mat4::identity()) {}
};

#endif /* TRANSFORM_H_ */
