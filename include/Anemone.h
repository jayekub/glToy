#ifndef ANEMONE_H_
#define ANEMONE_H_

#include "glToy.h"

#include "Prim.h"
#include "Scene.h"
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
    virtual void render(const Scene::State *state);

    void setMagnet(Vec3 position, float strength) {
        _magnetPosition = position;
        _magnetStrength = strength;
    }

private:
    int _numTentacles, _numSegments, _maxWidth, _numPts, _numLines;

    Vec3 *_tentacles, *_posed;

    Vec3 _currentDir;
    double _time;

    Vec3 _magnetPosition;
    float _magnetStrength;

    // XXX make static?
    Program _anemoneProgram;

    GLuint _p0Loc, _p1Loc, _lineWidthLoc,
           _cameraPosLoc, _viewportLoc, _fragToWorldLoc;

    GLuint _vertexBuffer, _indexBuffer, _p0Buffer, _p1Buffer;

    GLuint _lightPosLoc, _shadowMapLoc, _shadowMatrixLoc;
};

#endif /* ANEMONE_H_ */
