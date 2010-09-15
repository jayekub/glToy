// one point per particle in
layout(points) in;

// one quad per particle out
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

//uniform vec3 cameraPos;

in vec3 center[];
in float radius[];

out vec3 Pw;
flat out vec3 bcenter;
flat out float bradius;

void main()
{
    mat4 modelViewMat = viewMat * modelMat;
    mat4 viewProjMat = projMat * viewMat;

    vec3 Xw = vec3(viewMat[0][0],
                   viewMat[1][0],
                   viewMat[2][0]);

    vec3 Yw = vec3(viewMat[0][1],
                   viewMat[1][1],
                   viewMat[2][1]);

    vec3 Cw = ptransform(modelMat, center[0]);

    bcenter = Cw;
    bradius = radius[0];
    Pw = Cw - Xw - Yw;
    gl_Position = ptransform4(viewProjMat, Pw);
    EmitVertex();

    bcenter = Cw;
    bradius = radius[0];
    Pw = Cw - Xw + Yw;
    gl_Position = ptransform4(viewProjMat, Pw);
    EmitVertex();

    bcenter = Cw;
    bradius = radius[0];
    Pw = Cw + Xw - Yw;
    gl_Position = ptransform4(viewProjMat, Pw);
    EmitVertex();

    bcenter = Cw;
    bradius = radius[0];
    Pw = Cw + Xw + Yw;
    gl_Position = ptransform4(viewProjMat, Pw);
    EmitVertex();

    EndPrimitive();
}
