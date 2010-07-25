#ifndef SCENE_H_
#define SCENE_H_

#include <string>
#include <vector>

#include "Node.h"
#include "Visitor.h"

struct Scene : public Node
{
    void accept(Visitor *visitor) { visitor->visitScene(this); }

    Scene(const char *name) : Node(name) {};
};

#endif /* SCENE_H_ */
