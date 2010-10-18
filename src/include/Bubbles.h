#ifndef BUBBLE_H_
#define BUBBLE_H_

#include "RenderState.h"
#include "Program.h"
#include "ParticleSystem.h"

class Bubbles : public ParticleSystem
{
public:
    Bubbles(const char *name, const Vec3 &size, bool drawBox = false);
    virtual ~Bubbles();

private:
    Program _bubblesProgram;
    GLuint _permTexture, _gradTexture;

    bool _drawBox;
    Program _boxProgram;

    bool _particlelt(const Particle *a, const Particle * b,
                     const Vec3 &cameraPos) const;

    void _preRender(RenderState &state);
    void _postRender(RenderState &state);
};

#endif /* BUBBLE_H_ */
