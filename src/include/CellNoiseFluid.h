#ifndef _CELLNOISEFLUID_H_
#define _CELLNOISEFLUID_H_

#include "utils.h"
#include "Program.h"
#include "RenderState.h"
#include "ParticleSystem.h"

class ofxMSAFluidSolver;

// TODO refactor fluid influence as a Field
class CellNoiseFluid : public ParticleSystem<>
{
public:
    CellNoiseFluid(const char *name, const Vec2 &size,
                   ofxMSAFluidSolver *fluid, float particleSize);

    void update(double dt);

private:
    ofxMSAFluidSolver *_fluid;
    float _particleSize;

    Program _cellNoiseProgram;

    void _preRender(RenderState &state);
    void _postRender(RenderState &state);
};

#endif /* _CELLNOISEFLUID_H_ */
