#ifndef _VECFIELD_H_
#define _VECFIELD_H_

#include <math.h>

#include "Vec.h"

// see Numerical Recipes Ch. 5.7 Numerical Derivatives

// machine epsilon
#define EPS_M 1e-4

// cube root of EPS_M
#define EPS_M_3R 0.0464

// curvature scale of vector field
#define X_C(x) MAX(fabs(x), EPS_M)

// choose h to use in derivation and ensure it's representable
#define DECL_AND_SET_H(x) \
    float h; \
    { volatile float temp = x + EPS_M_3R*X_C(x); h = temp - x; }

class VecField
{
public:
    VecField(const Vec3 &size) { _allocate(size); }
    ~VecField() { _deallocate(); }

#define VAL(x_, y_, z_) _v[(int) ((z_)*_size.x*_size.y + (y_)*_size.x + (x_))]
#define VMIX(a, b, i) ((1. - (i))*(a) + (i)*(b))

#define CFLOOR(x, min) (MAX(floor(x), min))
#define CCEIL(x, max) (MIN(ceil(x), max))

    //// accessors do NOT perform bounds checking

    // write operator
    Vec3 &operator()(int x, int y, int z) {
        return VAL(x, y, z);
    }

    // lookup operator
    const Vec3 &operator()(int x, int y, int z) const {
        return VAL(x, y, z);
    }

    // trilinearly interpolated lookup operator
    Vec3 operator()(float x, float y, float z) const {
        int fx = CFLOOR(x, 0), fy = CFLOOR(y, 0), fz = CFLOOR(z, 0),
            cx = CCEIL(x, _size.x - 1.),
            cy = CCEIL(y, _size.y - 1.),
            cz = CCEIL(z, _size.z - 1.);

        const float xi = x - fx, yi = y - fy, zi = z - fz;

        // optimize for integral lookups
        if (fx == x && fy == y && fz == z)
            return VAL(x, y, z);

        // do interpolation
        // XXX may be slow, test/optimize
        return
            VMIX(
                VMIX(
                    VMIX(VAL(fx,fy,fz),VAL(cx,fy,fz),xi),
                    VMIX(VAL(fx,cy,fz),VAL(cx,cy,fz),xi),yi),
                VMIX(
                    VMIX(VAL(fx,fy,cz),VAL(cx,fy,cz),xi),
                    VMIX(VAL(fx,cy,cz),VAL(cx,cy,cz),xi), yi), zi);
    }

#undef VAL
#undef VMIX

    const Vec3 &getSize() const { return _size; }

    Vec3 pdx(float x, float y, float z) const {
        const VecField &me = *this;

        // central difference
        DECL_AND_SET_H(x);
        float x0 = MIN(x + h, _size.x - 1),
              x1 = MAX(x - h, 0);

        Vec3 F_x0 = me(x0, y, z), F_x1 = me(x1, y, z);
        Vec3 dFdx = (1./(x0 - x1))*(F_x0 - F_x1);

        /*
        if (x > _size.x - 2 && y > _size.y - 2 && z > _size.z - 2)
        if (x < 1 && y < 1 && z < 1)
            fprintf(stderr, "(%g, %g, %g), x0 = %g, x1 = %g, h = %g, F_x0 = %s, F_x1 = %s, dFdx = %s\n",
                    x, y, z, x0, x1, h,
                    F_x0.toString().c_str(),
                    F_x1.toString().c_str(),
                    dFdx.toString().c_str());
        */

        return dFdx;
    }

    Vec3 pdy(float x, float y, float z) const {
        const VecField &me = *this;

        DECL_AND_SET_H(y);
        const float y0 = MIN(y + h, _size.y - 1),
                    y1 = MAX(y - h, 0);

        return (1./(y0 - y1))*(me(x, y0, z) - me(x, y1, z));
    }

    Vec3 pdz(float x, float y, float z) const {
        const VecField &me = *this;

        DECL_AND_SET_H(z);
        const float z0 = MIN(z + h, _size.z - 1),
                    z1 = MAX(z - h, 0);

        return (1./(z0 - z1))*(me(x, y, z0) - me(x, y, z1));
    }

    Vec3 curl(float x, float y, float z) const {
        const Vec3 px = pdx(x, y, z),
                   py = pdy(x, y, z),
                   pz = pdy(x, y, z);

        return Vec3(py.z - pz.y, pz.x - px.z, px.y - py.x);
    }

    void randomize() {
        VecField &me = *this;

        for (int x = 0; x < _size.x; ++x)
            for (int y = 0; y < _size.y; ++y)
                for (int z = 0; z < _size.z; ++z)
                    me(x, y, z) = Vec3::randVec(-1, 1).normalize();
    }

private:
    Vec3 *_v;
    Vec3 _size;

    Vec3 _color;

    void _allocate(const Vec3 &size) {
        _size.x = ceil(size.x);
        _size.y = ceil(size.y);
        _size.z = ceil(size.z);

        _v = new Vec3[(int) (_size.x*_size.y*_size.z)];
    }

    void _deallocate() {
        delete [] _v;
    }
};

#undef DERIV_EPISLON

#endif // _VECFIELD_H_
