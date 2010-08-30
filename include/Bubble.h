#ifndef BUBBLE_H_
#define BUBBLE_H_

#include "Prim.h"
#include "RenderState.h"
#include "Program.h"

class Bubble : public Prim
{
public:
    Bubble(const char *name, float radius, int numBubbles = 1,
           int numLat = 10, int numLong = 10);
    virtual ~Bubble();

    void update(double dt);
    virtual void render(const RenderState &state);

private:
    float _radius;
    int _numBubbles, _numLat, _numLong;

    GLuint _vertexBuffer;

    Program _bubbleProgram;
    GLuint _permTexture, _gradTexture;
};

#endif /* BUBBLE_H_ */
