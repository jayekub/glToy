// one point per particle in
layout(points) in;

// up to four quads per particle out
//layout(triangle_strip, max_vertices = 16) out;
layout(points, max_vertices = 1) out;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

in vec3 center[];

void main()
{
    mat4 modelViewProjMat = projMat * viewMat * modelMat;

    gl_Position = ptransform4(modelViewProjMat, center[0]);

    EmitVertex();
    EndPrimitive();
}
