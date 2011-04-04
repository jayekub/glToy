#ifndef NOISE_H_
#define NOISE_H_

#include <values.h>
#include <stdlib.h>
#include <sys/types.h>

#include <cmath>

#include "Vec.h"
#include "glToy.h"

class Noise
{
public:
    // iq's value noise
    static float vnoise3f(float x, float y, float z) {
        int32_t X = (int) std::floor(x),
                Y = (int) std::floor(y),
                Z = (int) std::floor(z);

        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        float u = _fade(x),
              v = _fade(y),
              w = _fade(z);

        int32_t n = X + Y*57 + Z*113;

        float a = _randf(n + (0 + 57*0 + 113*0)),
              b = _randf(n + (1 + 57*0 + 113*0)),
              c = _randf(n + (0 + 57*1 + 113*0)),
              d = _randf(n + (1 + 57*1 + 113*0)),
              e = _randf(n + (0 + 57*0 + 113*1)),
              f = _randf(n + (1 + 57*0 + 113*1)),
              g = _randf(n + (0 + 57*1 + 113*1)),
              h = _randf(n + (1 + 57*1 + 113*1));

        float res =
            _lerp(w, _lerp(v, _lerp(u, a, b),
                              _lerp(u, c, d)),
                     _lerp(v, _lerp(u, e, f),
                              _lerp(u, g, h)));

        return 1.0 - res*(1./1073741824.);
    }

    static void dvnoise3f(float x, float y, float z,
                          float &N, float &dNdx, float &dNdy, float &dNdz) {

        int32_t X = (int) std::floor(x),
                Y = (int) std::floor(y),
                Z = (int) std::floor(z);

        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        float u = _fade(x),
              v = _fade(y),
              w = _fade(z),
              du = _dfade(x),
              dv = _dfade(y),
              dw = _dfade(z);

        int32_t n = X + Y*57 + Z*113;

        float a = _randf(n + (0 + 57*0 + 113*0))/1073741824.,
              b = _randf(n + (1 + 57*0 + 113*0))/1073741824.,
              c = _randf(n + (0 + 57*1 + 113*0))/1073741824.,
              d = _randf(n + (1 + 57*1 + 113*0))/1073741824.,
              e = _randf(n + (0 + 57*0 + 113*1))/1073741824.,
              f = _randf(n + (1 + 57*0 + 113*1))/1073741824.,
              g = _randf(n + (0 + 57*1 + 113*1))/1073741824.,
              h = _randf(n + (1 + 57*1 + 113*1))/1073741824.;

        float k0 =  a,
              k1 =  b - a,
              k2 =  c - a,
              k3 =  e - a,
              k4 =  a - b - c + d,
              k5 =  a - c - e + g,
              k6 =  a - b - e + f,
              k7 = -a + b + c - d + e - f - g + h;

        N = k0 + k1*u + k2*v + k3*w + k4*u*v + k5*v*w + k6*w*u + k7*u*v*w;
        dNdx = du*(k1 + k4*v + k6*w + k7*v*w);
        dNdy = dv*(k2 + k5*w + k4*u + k7*w*u);
        dNdz = dw*(k3 + k6*u + k5*v + k7*u*v);
    }

    // Port of Perlin's improved noise from reference implementation found
    // at http://mrl.nyu.edu/~perlin/noise/
    static double noise3d(double x, double y, double z) {
        // find unit cube containing the point
        int X = (int) std::floor(x) & 255,
            Y = (int) std::floor(y) & 255,
            Z = (int) std::floor(z) & 255;

        // find relative x,y,z of the point within the cube
        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

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
    }

private:

    // from iq
    inline static int32_t _randi(int32_t n) {
        n = (n << 13) ^ n;
        return (n*(n*n*15731 + 789221) + 1376312589) & 0x7fffffff;
    }

    inline static float _randf(int32_t n) {
        return ((float) _randi(n));
    }

    inline static double _fade(double t) {
        return t*t*t*(t*(t*6. - 15.) + 10.);
    }

    inline static double _dfade(double t) {
        return 30.*t*t*(t*(t-2.)+1.);
    }

    inline static double _lerp(double t, double a, double b) {
        return a + t*(b - a);
    }

    inline static double _grad(int hash, double x, double y, double z) {
        // convert low 4 bits of hash code into 12 gradient directions
        int h = hash & 15;
        double u = h < 8 ? x : y,
               v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    static int _p[512];
};

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

#endif // NOISE_H_
