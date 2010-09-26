// one point per particle in
layout(points) in;

// up to four quads per particle out
layout(triangle_strip, max_vertices = 16) out;
//layout(points, max_vertices = 16) out;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform vec2 rasterSize;
uniform float particleSize;

in vec3 center[];

out vec2 texcoord;

void main()
{
    mat4 vpMat = projMat * viewMat;

    // particle radius in world space
    float psW = particleSize * rasterSize.x;

    // relative quad corners in world space
    vec3 cornersW[4];

    cornersW[0] = psW * vec3(-0.5, -0.5, 0.);
    cornersW[1] = psW * vec3( 0.5, -0.5, 0.);
    cornersW[2] = psW * vec3( 0.5,  0.5, 0.);
    cornersW[3] = psW * vec3(-0.5,  0.5, 0.);

    // relative quad corners in clip space
    vec4 cornersC[4];

    for (int i = 0; i < 4; ++i) {
        cornersC[i] = vtransform4(vpMat, cornersW[i]);
    }

    // texture coordinates for each corner
    vec2 texcoords[4];

    texcoords[0] = vec2(0., 0.);
    texcoords[1] = vec2(1., 0.);
    texcoords[2] = vec2(1., 1.);
    texcoords[3] = vec2(0., 1.);

#define EMIT_VERT(centerC, n) \
    texcoord = texcoords[n]; \
    gl_Position = centerC + cornersC[n]; \
    EmitVertex();

#define EMIT_QUAD(centerC) \
    EMIT_VERT(centerC, 0); \
    EMIT_VERT(centerC, 1); \
    EMIT_VERT(centerC, 3); \
    EMIT_VERT(centerC, 2); \
    EndPrimitive();

    vec3 qw = ptransform(modelMat, center[0]);
    vec4 qc = ptransform4(vpMat, qw);

    EMIT_QUAD(qc);

    // for swizzling
    vec3 rsW = vec3(rasterSize, 0);
    vec3 rsC = vtransform(vpMat, rsW);

    float ps_2 = psW / 2.;

    // draw wrapped versions of the quad for tiling
    vec4 qx = qc, qy = qc;

    /* XXX why is this slow?? appears to be better to branch and not emit
           geometry than to avoid branches and always emit...

    qx += (qw.x < ps_2 ? 1. : qw.x > rsW.x - ps_2 ? -1. : 0.) * rsC.xzzz;
    EMIT_QUAD(qx);

    qy += (qw.y < ps_2 ? 1. : qw.y > rsW.y - ps_2 ? -1. : 0.) * rsC.zyzz;
    EMIT_QUAD(qy);

    qc = vec4(qx.x, qy.y, qc.z, qc.w);
    EMIT_QUAD(qc);
    */

    if (qw.x < ps_2) {
        qx += rsC.xzzz;
        EMIT_QUAD(qx);
    } else if (qw.x > rsW.x - ps_2) {
        qx -= rsC.xzzz;
        EMIT_QUAD(qx);
    }

    if (qw.y < ps_2) {
        qy += rsC.zyzz;
        EMIT_QUAD(qy);
    } else if (qw.y > rsW.y - ps_2) {
        qy -= rsC.zyzz;
        EMIT_QUAD(qy);
    }

    if (qx != qc && qy != qc) {
        qc = vec4(qx.x, qy.y, qc.zw);
        EMIT_QUAD(qc);
    }
}
