#include <boost/foreach.hpp>

#include "Node.h"
#include "Transform.h"
#include "Graph.h"

Graph::Graph(const char *name_) : name(name_)
{
    root = new Transform("rootTransform");
    registerNode(root);
};

Graph::~Graph()
{
    BOOST_FOREACH(_NodeMap::value_type graphPair, _graphNodes) {
        delete graphPair.second;
    }
}

void Graph::addGlobal(Node *node)
{
    globals.push_back(node);
    registerNode(node);
}

void Graph::registerNode(Node *node)
{
    _graphNodes[node->name] = node;
    node->_graph = this;
}

Node *Graph::getNode(const char *name) const
{
    _NodeMap::const_iterator nodeIt = _graphNodes.find(name);
    return (nodeIt == _graphNodes.end()) ? NULL : nodeIt->second;
}
