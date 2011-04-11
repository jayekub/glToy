#ifndef _NODE_H_
#define _NODE_H_

#include <string>
#include <vector>

#include "ptr.h"

#define DEF_NODE_CREATE(type) \
    static type##Ptr create(const std::string &name_) { \
        return type##Ptr(new type(name_)); }

struct Graph;
class Visitor;

DEF_SHARED_PTR(Node);

struct Node
{
    std::string name;
    std::vector<NodeConstPtr> children;

    virtual void accept(boost::scoped_ptr<Visitor> &visitor) const = 0;
    virtual void addChild(const NodePtr &child);

protected:
    Node(const std::string &name_) : name(name_) { };

private:
    friend struct Graph;
    boost::shared_ptr<Graph> _graph;
};

#endif // _NODE_H_
