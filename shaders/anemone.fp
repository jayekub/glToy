//attribute vec3 p0, p1;

void main()
{
    gl_FragColor = mix(vec4(1., 1., 0.7, 1.), vec4(.3, .3, .4, 1.),
                       gl_FragCoord.z * gl_FragCoord.z);

//    gl_FragColor = vec4(1., 0., 0., 1.);
}