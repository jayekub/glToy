const int blurRadius = 10;
const int kernelWidth = 2 * blurRadius + 1;

uniform sampler2D tex0;

uniform float blurWeights[kernelWidth];
uniform vec2 dxdy;
uniform bool horizontal;

in vec2 texcoord;

out vec4 color;

void main()
{
    color = vec4(0.);

    vec2 offset = horizontal ? vec2(dxdy.x, 0.) : vec2(0., dxdy.y);

    for (int i = 0; i < kernelWidth; ++i)
        color += blurWeights[i] * 
                 texture(tex0, texcoord + (i - blurRadius - 1) * offset);
}
