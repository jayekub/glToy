uniform float meanRadius;
uniform float radiusSpread;

in vec2 ptexcoord;
flat in vec3 pcenter;
flat in float pspeed;
flat in float pradius;

out vec4 color;

void main()
{
#if 1
    vec2 v = ptexcoord.st - vec2(0.5, 0.5);
    //vec2 v = gl_PointCoord.st - vec2(0.5, 0.5);
    float len = length(v);

    float index = ((pradius - meanRadius)/radiusSpread) + 0.5;

    /*
    vec4 baseColor = smoothtriad(vec4(0.56, 0.56, 0.76, 1.),
                                 vec4(0.56, 0.76, 0.56, 1.),
                                 vec4(0.76, 0.56, 0.56, 1.),
                                 index);
    */

    vec4 baseColor = index*vec4(0.56, 0.56, 0.76, 1.);

    color = vec4(baseColor.rgb, .6 * (1. - smoothstep(0.4, 0.5, len)));
    //color.a *= 100/pspeed;

    if (len > 0.5) discard;
#else
    color = vec4(gl_PointCoord.st, 1, 1);
    //color = vec4(ptexcoord.st, 1, 1);
#endif
}
