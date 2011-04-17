layout(points) in;
layout(points, max_vertices = 128) out;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform float time;

in vec3 vertex[];

float wavefBm(in vec3 p)
{
    float result = 0;

    result += 0.5000*noise3f(0.5*p);
    result += 0.2500*noise3f(1.*p);

    return result;
}

void main()
{
    vec3 Pnoise = (vertex[0] + 10*time*vec3(2., 1., 1.))/10.;

    float height = 10.*wavefBm(Pnoise);

    vec3 Po = vertex[0] + vec3(0, height, 0);

    gl_Position = projMat * viewMat * modelMat * vec4(Po, 1.);
    EmitVertex();
    EndPrimitive();
}
