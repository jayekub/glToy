void main()
{
    float d = length(vec2(0.5, 0.5) - gl_TexCoord[0].st);
    
    gl_FragColor = vec4(1.0 - d, 1.0 - d, d, 1.0);
    gl_FragDepth = d;
}