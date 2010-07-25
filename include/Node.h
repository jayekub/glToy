#ifndef NODE_H_
#define NODE_H_

#include <vector>

class Visitor;

struct Node
{
    const char *name;
    std::vector<Node *> children;

    virtual void accept(Visitor *visitor) = 0;

    virtual void addChild(Node *child) { children.push_back(child); }

    Node(const char *name_) : name(name_) {};
};

#endif // NODE_H_
