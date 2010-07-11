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
                      double particleSize = 50.0);

    void reload();

    void setNoiseParticles(ParticleSystem *noiseParticles);

private:
    Program _cellNoiseProgram;

    ParticleSystem *_noiseParticles;
    double _particleSize;
    double _ps_2, _maxD;

    virtual void _render(RenderPass *renderPass);
    void _drawParticle(double x, double y);
};

#endif /* CELLNOISERENDERPASS_H_ */
