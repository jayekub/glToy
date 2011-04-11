#include <boost/foreach.hpp>

#include "Node.h"
#include "Visitor.h"

void
Visitor::_visitChildren(const NodeConstPtr &node)
{
    //fprintf(stderr, "visiting %s\n", node->name);
    //PRINT_GLERROR(node->name);
    _visitNodes(node->children);
}

void
Visitor::_visitNodes(const std::vector<NodeConstPtr> &nodes)
{
    VisitorPtr me(this);

    BOOST_FOREACH(NodeConstPtr node, nodes) {
        node->accept(me);
    }
}

void
Visitor::_visitNodes(const std::vector<NodeConstWeakPtr> &nodes)
{
    VisitorPtr me(this);

    BOOST_FOREACH(NodeConstWeakPtr weakNode, nodes) {
        if (!weakNode.expired()) {
            NodeConstPtr node(weakNode);
            node->accept(me);
        }
    }
}
