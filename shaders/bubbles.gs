// one point per particle in
layout(points) in;

// one uuad per particle out
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform vec3 cameraPos;

in vec3 center[];
in float radius[];

out vec3 Pw;
flat out vec3 bcenter;
flat out float bradius;

void main()
{
    mat4 modelViewMat = viewMat * modelMat;
    mat4 viewProjMat = projMat * viewMat;

    vec3 centerW = ptransform(modelMat, center[0]);

    // world oriented
//    vec3 Xw = radius[0] * vec3(1., 0., 0.);
//    vec3 Yw = radius[0] * vec3(0., 1., 0.);
//    vec3 Zw = radius[0] * vec3(0., 0., 1.);

    // viewplane oriented
//    vec3 Xw = radius[0] * vec3(viewMat[0][0], viewMat[1][0], viewMat[2][0]);
//    vec3 Yw = radius[0] * vec3(viewMat[0][1], viewMat[1][1], viewMat[2][1]);
//    vec3 Zw = radius[0] * vec3(viewMat[0][2], viewMat[1][2], viewMat[2][2]);

    // viewpoint oriented
    vec3 Vw = normalize(centerW - cameraPos);
    vec3 UPw = vec3(0., 1., 0.);

    vec3 Xw = radius[0] * normalize(cross(UPw, Vw));
    vec3 Yw = radius[0] * normalize(cross(Vw, Xw));
    vec3 Zw = radius[0] * Vw;

    // compute quad corners
    vec3 cornersW[4];

    cornersW[0] = centerW - Xw - Yw - Zw;
    cornersW[1] = centerW + Xw - Yw - Zw;
    cornersW[2] = centerW + Xw + Yw - Zw;
    cornersW[3] = centerW - Xw + Yw - Zw;

    // transform corners to clip space and check for intersections with the
    // clipping planes--discard if there are any to avoid mangled particles

    bool discardParticle = false;
    vec4 cornersC[4];

    for (int i = 0; i < 4; ++i) {
        cornersC[i] = ptransform4(viewProjMat, cornersW[i]);

#if 1
        float cornerZNDC = cornersC[i].z / cornersC[i].w;
        if (cornerZNDC < -1. || cornerZNDC > 1.) {
            discardParticle = true;
            break;
        }
#endif
    }

    // now emit the box
    if (!discardParticle) {

#define EMIT_VERT(n) \
    bcenter = centerW; \
    bradius = radius[0]; \
    Pw = cornersW[n]; \
    gl_Position = cornersC[n]; \
    EmitVertex();

        EMIT_VERT(0); EMIT_VERT(1); EMIT_VERT(3); EMIT_VERT(2);

        EndPrimitive();

    }
}
