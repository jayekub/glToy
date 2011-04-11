#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <string>
#include <unordered_map>

#include "ptr.h"
#include "Node.h"

DEF_SHARED_PTR(Graph);

struct Graph
{
    std::string name;

    // Global nodes are special nodes that should be visited before other
    // children because they setup global state
    std::vector<NodeConstWeakPtr> globals;

    NodePtr root;

    void addGlobal(const NodePtr &node);

    void registerNode(const NodePtr &node);
    NodeConstWeakPtr getNode(const std::string &name) const;

    static GraphPtr create(const std::string &name_) {
        return GraphPtr(new Graph(name_));

private:
    typedef std::unordered_map<std::string, NodeConstWeakPtr> _NodeMap;
    _NodeMap _graphNodes;

    Graph(const std::string &name_);
};

#endif // _GRAPH_H_
