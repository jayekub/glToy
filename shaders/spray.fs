uniform float meanRadius;
uniform float radiusSpread;

out vec4 color;

//in vec3 velocity;
in float radius;

void main()
{
#if 0
    vec2 v = gl_PointCoord.st - vec2(0.5, 0.5);
    float len = length(v);

    float index = ((radius - meanRadius)/radiusSpread) + 0.5;

    /*
    vec4 baseColor = smoothtriad(vec4(0.56, 0.56, 0.76, 1.),
                                 vec4(0.56, 0.76, 0.56, 1.),
                                 vec4(0.76, 0.56, 0.56, 1.),
                                 index);
    */

    vec4 baseColor = index*vec4(0.56, 0.56, 0.76, 1.);

    color = vec4(baseColor.rgb, .6 * (1. - smoothstep(0.4, 0.5, len)));

    if (len > 0.5) discard;
#else
    color = vec4(gl_PointCoord.st, 1, 1);
#endif
}
