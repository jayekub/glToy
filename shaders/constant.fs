uniform vec3 color;

out vec4 colorOut;

void main()
{
    colorOut = vec4(color, 1.);
}
