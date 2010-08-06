#ifndef SCENE_H_
#define SCENE_H_

#include <map>

#include <boost/foreach.hpp>

#include "Node.h"
#include "Visitor.h"
#include "Vec.h"

class RenderPass;

struct Scene : public Node
{
    struct State {
        RenderPass *renderPass;

        Camera *camera;

        // XXX use hash_map
        std::map<const char *, Light *> lights;
        std::map<const char *, Vec3> lightPositions;

        void reset() {
            renderPass = NULL;
            camera = NULL;

            lights.clear();
            lightPositions.clear();
        };
    };

    State state;

    // Global nodes are special nodes that should be visited before other
    // children because they setup global state
    std::vector<Node *> globals;

    void accept(Visitor *visitor) { visitor->visitScene(this); }

    void addGlobal(Node *node) { globals.push_back(node); }

    Scene(const char *name) : Node(name) {};
    ~Scene() {
        BOOST_FOREACH(Node *child, globals) {
            delete child;
        }
    }
};

#endif /* SCENE_H_ */
