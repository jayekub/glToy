#ifndef BUBBLE_H_
#define BUBBLE_H_

#include "RenderState.h"
#include "Program.h"
#include "ParticleSystem.h"

struct BubbleAttributes
{
    float radius;
};

class Bubbles : public ParticleSystem<Particle<BubbleAttributes> >
{
public:
    Bubbles(const char *name, const Vec3 &size, float radius,
            bool drawBox = false);
    virtual ~Bubbles();

private:
    float _radius;

    Program _bubblesProgram;
    GLuint _permTexture, _gradTexture;

    bool _drawBox;
    Program _boxProgram;

    void _setRandomAttributes(particle_t *p) const;

    bool _particlelt(const particle_t *a, const particle_t * b,
                     const Vec3 &cameraPos) const;

    void _preRender(RenderState &state);
    void _postRender(RenderState &state);
};

#endif /* BUBBLE_H_ */
