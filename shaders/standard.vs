uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

in vec3 vertexIn;
in vec2 texcoordIn;

out vec2 texcoord;

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(vertexIn, 1.);
    texcoord = texcoordIn;
}