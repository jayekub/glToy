uniform sampler2D tex0;

in vec2 texcoord;

out vec4 color;

void main()
{
    color = texture(tex0, texcoord);
}