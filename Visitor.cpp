#include <boost/foreach.hpp>

#include "Node.h"
#include "Visitor.h"

Visitor::Visitor()
{
    // start off with identity ctm
    _transformStack.push(Mat4());
}

void
Visitor::_visitChildren(Node *node)
{
    _visitNodes(node->children);
}

void
Visitor::_visitNodes(const std::vector<Node *> &nodes)
{
    BOOST_FOREACH(Node *node, nodes) {
        node->accept(this);
    }
}

void
Visitor::_pushTransform()
{
    //_transformStack.push(_currentTransform);
    _transformStack.push(_transformStack.top());
}

void
Visitor::_popTransform()
{
    //_currentTransform = _transformStack.top();
    _transformStack.pop();
}

void
Visitor::_multTransform(const Mat4 &mat)
{
    //_currentTransform *= mat;
    _transformStack.top() *= mat;
}
