uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

in vec2 texcoord;

out vec4 color;

void main()
{
    vec4 sharp = texture(tex0, texcoord);
    float depth = texture(tex1, texcoord).r;
    vec4 blurred = texture(tex2, texcoord);

    color = mix(sharp, blurred, max(0, 2. * depth * depth - 1));
    //color = texture(tex2, texcoord);
}
