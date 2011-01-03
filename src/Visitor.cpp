#include <boost/foreach.hpp>

#include "Node.h"
#include "Visitor.h"

Visitor::Visitor()
{
}

void
Visitor::_visitChildren(Node *node)
{
    //fprintf(stderr, "visiting %s\n", node->name);
    //PRINT_GLERROR(node->name);
    _visitNodes(node->children);
}

void
Visitor::_visitNodes(const std::vector<Node *> &nodes)
{
    BOOST_FOREACH(Node *node, nodes) {
        node->accept(this);
    }
}
