#ifndef BUBBLE_H_
#define BUBBLE_H_

#include "RenderState.h"
#include "Program.h"
#include "ParticleSystem.h"

class Bubbles : public ParticleSystem<Vec3>
{
public:
    Bubbles(const char *name, const Vec3 &size,
            float radius, int numLat = 5, int numLong = 5);
    virtual ~Bubbles();

private:
    float _radius;
    int _numLat, _numLong;

    Program _bubblesProgram;
    GLuint _permTexture, _gradTexture;

    void _prepRender(const RenderState &state);
};

#endif /* BUBBLE_H_ */
