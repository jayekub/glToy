#ifndef ANEMONE_H_
#define ANEMONE_H_

#include "Vec.h"
#include "Prim.h"

class Anemone : public Prim
{
public:
    Anemone(int numTentacles, int numSegments, double wiggle = 0.03);
    virtual ~Anemone();

    void update(double dt);
    void render();

private:
    int _numTentacles, _numSegments;
    Vec3 **_tentacles;

    Vec3 _currentDir;
    double **_currentOffsets;
    double _time;

    void _destroy();

};

#endif /* ANEMONE_H_ */
