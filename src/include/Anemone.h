#ifndef ANEMONE_H_
#define ANEMONE_H_

#include "glToy.h"
#include "ptr.h"
#include "RenderState.h"
#include "Node.h"
#include "Prim.h"
#include "Program.h"
#include "Vec.h"

DEF_SHARED_PTR(Anemone);

class Anemone : public Prim
{
public:
    void update(double dt);
    virtual void render(RenderState &state);

    void setMagnet(Vec3 position, float strength) {
        _magnetPosition = position;
        _magnetStrength = strength;
    }

    DEF_NODE_CREATE(Anemone);

private:
    // XXX turn params into public fields
    Anemone(const std::string &name_,
            int numTentacles = 25, int numSegments = 10,
            int maxWidth = 5., double wiggle = 0.03);
    virtual ~Anemone();

    int _numTentacles, _numSegments, _maxWidth, _numPts, _numLines;

    Vec3 _currentDir;
    double _time;

    Vec3 _magnetPosition;
    float _magnetStrength;

    // XXX make static?
    Program _anemoneProgram;
    GLuint _permTexture, _gradTexture;

    GLuint _vertexBuffer, _pointNumBuffer;
};

#endif /* ANEMONE_H_ */
