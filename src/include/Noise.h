#ifndef NOISE_H_
#define NOISE_H_

#include <math.h>

#include "glToy.h"

// This implementation is from Stefan Gustavson and was found at
// http://staffwww.itn.liu.se/~stegu/simplexnoise/
class GLSLNoise
{
public:
    static GLuint makePermutationTexture();
    static GLuint makeGradientTexture();

private:
    static int _perm[256];
    static int _grad3[16][3];
    static int _grad4[32][4];
};

// Port of Perlin's improved noise from reference implementation found
// at http://mrl.nyu.edu/~perlin/noise/
class ClassicNoise
{
public:
    static double noise3(double x, double y, double z) {
        // find unit cube containing the point
        int X = (int) floor(x) & 255,
            Y = (int) floor(y) & 255,
            Z = (int) floor(z) & 255;

        // find relative x,y,z of the point within the cube
        x -= floor(x);
        y -= floor(y);
        z -= floor(z);

        // compute fade curves
        double u = _fade(x),
               v = _fade(y),
               w = _fade(z);

        // hash coordinates of the 8 cube corners
        int A = _p[X  ]+Y, AA = _p[A]+Z, AB = _p[A+1]+Z,
            B = _p[X+1]+Y, BA = _p[B]+Z, BB = _p[B+1]+Z;

        // and add blended results from 8 corners of cube
        return _lerp(w, _lerp(v, _lerp(u, _grad(_p[AA  ], x  , y  , z   ),
                                          _grad(_p[BA  ], x-1, y  , z   )),
                                 _lerp(u, _grad(_p[AB  ], x  , y-1, z   ),
                                          _grad(_p[BB  ], x-1, y-1, z   ))),
                        _lerp(v, _lerp(u, _grad(_p[AA+1], x  , y  , z-1 ),
                                          _grad(_p[BA+1], x-1, y  , z-1 )),
                                 _lerp(u, _grad(_p[AB+1], x  , y-1, z-1 ),
                                          _grad(_p[BB+1], x-1, y-1, z-1 ))));
    };

private:
    inline static double _fade(double t) {
        return t * t * t * (t * (t * 6. - 15.) + 10.);
    };

    inline static double _lerp(double t, double a, double b) {
        return a + t * (b - a);
    };

    inline static double _grad(int hash, double x, double y, double z) {
        // convert low 4 bits of hash code into 12 gradient directions
        int h = hash & 15;
        double u = h < 8 ? x : y,
               v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    static int _p[512];
};

#endif // NOISE_H_
