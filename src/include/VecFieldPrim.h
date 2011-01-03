#ifndef _VECFIELDPRIM_H_
#define _VECFIELDPRIM_H_

#include "RenderState.h"
#include "Program.h"
#include "Vec.h"

#include "Prim.h"

class VecField;

class VecFieldPrim : public Prim
{
public:
    VecFieldPrim(const char *name, const VecField *vecField);
    ~VecFieldPrim();

    void render(RenderState &state);

    void setColor(const Vec3 &color) { _color = color; }

private:
    const VecField *_vecField;

    Vec3 _color;

    GLuint _vecBuffer;
    Program _vecProgram;
};

#endif // _VECFIELDPRIM_H_
