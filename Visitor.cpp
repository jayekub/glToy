#include <boost/foreach.hpp>

#include "Node.h"
#include "Visitor.h"

void
Visitor::_visitChildren(Node *node)
{
    BOOST_FOREACH(Node *child, node->children) {
        child->accept(this);
    }
}
