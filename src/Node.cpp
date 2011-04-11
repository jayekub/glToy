#include "Graph.h"
#include "Node.h"

void Node::addChild(const NodePtr &child) {
    children.push_back(child);
    _graph->registerNode(child);
}
