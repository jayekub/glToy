#ifndef _VISITOR_H_
#define _VISITOR_H_

#include <vector>
#include <stack>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "ptr.h"

struct Node;
struct Camera;
struct Light;
struct Transform;
class Prim;
class Visitor;

DEF_SCOPED_PTR(Visitor);

class Visitor {
public:
    virtual void visitCamera(
        const boost::shared_ptr<const Camera> &camera) = 0;

    virtual void visitLight(
        const boost::shared_ptr<const Light> &light) = 0;

    virtual void visitTransform(
        const boost::shared_ptr<const Transform> &transform) = 0;

    virtual void visitPrim(
        const boost::shared_ptr<const Prim> &prim) = 0;

protected:
    void _visitChildren(const boost::shared_ptr<const Node> &node);
    void _visitNodes(const std::vector<boost::shared_ptr<const Node> > &nodes);
    void _visitNodes(const std::vector<boost::weak_ptr<const Node> > &nodes);
};

#endif // _VISITOR_H_
