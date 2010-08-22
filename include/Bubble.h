#ifndef BUBBLE_H_
#define BUBBLE_H_

#include "Prim.h"
#include "RenderState.h"
#include "Program.h"

class Bubble : public Prim
{
public:
    Bubble(const char *name, float radius, int numLat = 10, int numLong = 10);
    virtual ~Bubble();

    void update(double dt);
    virtual void render(const RenderState &state);

private:
    float _radius;
    int _numLat, _numLong;

    Program _bubbleProgram;


};

#endif /* BUBBLE_H_ */
