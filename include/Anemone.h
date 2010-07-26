#ifndef ANEMONE_H_
#define ANEMONE_H_

#include "Prim.h"
#include "Program.h"
#include "Vec.h"

class Anemone : public Prim
{
public:
    Anemone(const char *name,
            int numTentacles, int numSegments, double wiggle = 0.03);
    virtual ~Anemone();

    void update(double dt);
    void render();

private:
    int _numTentacles, _numSegments, _numPts, _numLines;

    Vec3 *_tentacles, *_vertices;

    Vec3 _currentDir;
    double _time;

    // XXX make static?
    Program _anemoneProgram;

    GLuint _vertexBuffer, _indexBuffer;

    void _destroy();
};

#endif /* ANEMONE_H_ */
