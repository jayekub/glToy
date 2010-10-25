in float radius;

out vec4 color;

void main()
{
    vec2 v = gl_PointCoord.st - vec2(0.5, 0.5);
    float len = length(v);

    color = vec4(0.56, 0.56, 0.76, .6 * (1. - smoothstep(0.4, 0.5, len)));

    if (len > 0.5) discard;
}
