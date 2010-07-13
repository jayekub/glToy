#ifndef CELLNOISERENDERPASS_H_
#define CELLNOISERENDERPASS_H_

#include "Renderer.h"
#include "RenderPass.h"
#include "Program.h"
#include "ParticleSystem.h"

class CellNoiseRenderer : public Renderer
{
public:
    CellNoiseRenderer(ParticleSystem *noiseParticles,
                      double particleSize = 0.1);

    void render(RenderPass *renderPass = NULL,
                ParticleSystem *noiseParticles = NULL,
                double particleSize = -1.0);
    void reload();

private:
    Program _cellNoiseProgram;

    ParticleSystem *_noiseParticles;
    double _particleSize;

    GLuint _particleDisplayList;

    virtual void _render(RenderPass *renderPass);
    void _drawParticle(double x, double y, double size);
};

#endif /* CELLNOISERENDERPASS_H_ */
