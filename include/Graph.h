#ifndef GRAPH_H_
#define GRAPH_H_

#include <unordered_map>

class Node;

struct Graph
{
private:
    typedef std::unordered_map<const char *, Node *> _NodeMap;
    _NodeMap _graphNodes;

public:
    const char *name;

    // Global nodes are special nodes that should be visited before other
    // children because they setup global state
    std::vector<Node *> globals;

    Node *root;

    void addGlobal(Node *node);

    void registerNode(Node *node);
    Node *getNode(const char *name) const;

    Graph(const char *name_);
    ~Graph();
};

#endif /* GRAPH_H_ */
