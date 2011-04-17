#ifndef _WAVEPRIM_H_
#define _WAVEPRIM_H_

#include "Prim.h"
#include "RenderState.h"
#include "Program.h"

class WavePrim : public Prim
{
public:
    WavePrim(const char *name, int size = 100);
    ~WavePrim();

    void update(double dt);
    void render(RenderState &state);

private:
    int _size;

    double _time;

    GLuint _vertexBuffer;
    Program _waveProgram;
};

#endif // _WAVEPRIM_H_
