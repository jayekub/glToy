void main()
{
    // normalize so that the max distance is reached perpendicular to quad
    // edges, clamp to prevent corners of quad from going over.
    
    float d = clamp(length(vec2(0.5, 0.5) - gl_TexCoord[0].st) / 0.5, 0.0, 1.0);
    
    gl_FragColor = vec4(d, d, d, 1.0);
    gl_FragDepth = d;
}