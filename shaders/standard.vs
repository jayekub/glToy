uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

in vec3 vertexIn;
in vec2 texcoordIn;

out vec3 vertex;
out vec2 texcoord;

void main()
{
    vertex = vertexIn;
    texcoord = texcoordIn;

    gl_Position = projMat * viewMat * modelMat * vec4(vertexIn, 1.);
}
