#include <boost/foreach.hpp>

#include "Node.h"
#include "Visitor.h"

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
