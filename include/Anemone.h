#ifndef ANEMONE_H_
#define ANEMONE_H_

#include "glToy.h"

#include "Prim.h"
#include "Program.h"
#include "Vec.h"

class Anemone : public Prim
{
public:
    Anemone(const char *name,
            int numTentacles, int numSegments,
            int maxWidth = 5., double wiggle = 0.03);
    virtual ~Anemone();

    void update(double dt);
    void render();

private:
    int _numTentacles, _numSegments, _maxWidth, _numPts, _numLines;

    Vec3 *_tentacles, *_posed;

    Vec3 _currentDir;
    double _time;

    // XXX make static?
    Program _anemoneProgram;

    GLuint _p0Loc, _p1Loc, _lineWidthLoc,
           _cameraPosLoc, _viewportLoc, _fragToWorldLoc;

    GLuint _vertexBuffer, _indexBuffer, _p0Buffer, _p1Buffer;
};

#endif /* ANEMONE_H_ */
