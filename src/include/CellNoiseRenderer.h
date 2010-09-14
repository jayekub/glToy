#ifndef CELLNOISERENDERPASS_H_
#define CELLNOISERENDERPASS_H_

#include "Renderer.h"
#include "RenderPass.h"
#include "Program.h"
#include "ParticleSystem.h"

class CellNoiseRenderer : public Renderer, Listener
{
public:
    CellNoiseRenderer();

    void render(RenderPass *renderPass,
                ParticleSystem<> *noiseParticles,
                float particleSize = -1.0);
    void reload();

private:
    Program _cellNoiseProgram;

    GLuint _particleDisplayList;

    void _drawParticle(float x, float y, float size);
};

#endif /* CELLNOISERENDERPASS_H_ */
