#include <boost/foreach.hpp>

#include "Transform.h"
#include "Graph.h"

Graph::Graph(const std::string &name_) :
    name(name_), root(TransformPtr(new Transform("rootTransform")))
{
    registerNode(root);
};

void Graph::addGlobal(const NodePtr &node)
{
    globals.push_back(node);
    registerNode(node);
}

void Graph::registerNode(const NodePtr &node)
{
    _graphNodes[node->name] = NodeConstWeakPtr(node);
    node->_graph = GraphPtr(this);
}

NodeConstWeakPtr Graph::getNode(const std::string &name) const
{
    _NodeMap::const_iterator nodeIt = _graphNodes.find(name);
    return (nodeIt == _graphNodes.end()) ? NodeWeakPtr() : nodeIt->second;
}
