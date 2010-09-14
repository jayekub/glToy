#ifndef NODE_H_
#define NODE_H_

#include <vector>

#include <boost/foreach.hpp>

#include "Graph.h"

class Visitor;

struct Node
{
private:
    friend class Graph;
    Graph *_graph;

public:
    const char *name;
    std::vector<Node *> children;

    virtual void accept(Visitor *visitor) = 0;

    virtual void addChild(Node *child) {
        children.push_back(child);
        _graph->registerNode(child);
    }

    Node(const char *name_) : name(name_) {};
    virtual ~Node() {};


};

#endif // NODE_H_
