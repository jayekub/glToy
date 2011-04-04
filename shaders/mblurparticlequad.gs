// one point per particle in
layout(points) in;

// one quad per particle out
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform vec3 cameraPos;
uniform float dt;

in vec3 center[];
in float radius[];
in vec3 velocity[];

out vec3 Pw;
out vec2 ptexcoord;
flat out vec3 pcenter;
flat out float pspeed;
flat out float pradius;

void main()
{
    mat4 modelViewMat = viewMat * modelMat;
    mat4 modelViewProjMat = projMat * modelViewMat;
    mat4 viewProjMat = projMat * viewMat;

    vec3 centerW = ptransform(modelMat, center[0]);

    // viewpoint oriented quad
    vec3 Vw = normalize(centerW - cameraPos);
    vec3 UPw = vec3(0., 1., 0.);

    vec3 Xw = normalize(cross(UPw, Vw));
    vec3 Yw = normalize(cross(Vw, Xw));

    // project particle velocity into quad
    vec3 VelW = vtransform(modelMat, velocity[0]);
    vec3 VelQ = dot(VelW, Xw)*Xw + dot(VelW, Yw)*Yw; 
    float SpeedQ = 10.*length(VelQ); // XXX fudge factor

    // realign and resize quad so that X axis is along velocity dir, X
    // length is dt*SpeedQ + radius, and Y length is radius
    Xw = (radius[0] + dt*SpeedQ)*normalize(VelQ);
    Yw = radius[0]*normalize(cross(Vw, Xw));

    vec3 Zw = Vw; // offset from center by radius to account for
                  // spherical particles

    // X axis of quad is along particle motion in screen space
    //vec3 VelS = vtransform(modelViewProjMat, velocity[0]);
    //float SpeedS = length(VelS);

    // compute quad corners and texcoords
    vec3 cornersW[4];
    vec2 texcoord[4];

    cornersW[0] = centerW - Xw - Yw - Zw;
    cornersW[1] = centerW + Xw - Yw - Zw;
    cornersW[2] = centerW + Xw + Yw - Zw;
    cornersW[3] = centerW - Xw + Yw - Zw;

    texcoord[0] = vec2(1., 1.);
    texcoord[1] = vec2(0., 1.);
    texcoord[2] = vec2(0., 0.);
    texcoord[3] = vec2(1., 0.);

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
    pcenter = centerW; \
    pspeed = SpeedQ; \
    pradius = radius[0]; \
    ptexcoord = texcoord[n]; \
    Pw = cornersW[n]; \
    gl_Position = cornersC[n]; \
    EmitVertex();

        EMIT_VERT(0); EMIT_VERT(1); EMIT_VERT(3); EMIT_VERT(2);

        EndPrimitive();

    }
}
