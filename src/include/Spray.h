#ifndef SPRAY_H_
#define SPRAY_H_

#include "RenderState.h"
#include "Program.h"
#include "ParticleSystem.h"

class Spray : public ParticleSystem
{
public:
    Spray(const char *name, const Vec3 &size);
    virtual ~Spray();

private:
    float _radius;

    Program _sprayProgram;

    void _preRender(RenderState &state);
    void _postRender(RenderState &state);
};

#endif /* SPRAY_H_ */
