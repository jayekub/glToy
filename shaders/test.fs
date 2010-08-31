uniform sampler2D tex0;

void main()
{
    vec4 c = vec4(0.); //texture2D(tex0, gl_TexCoord[0].st);

    gl_FragColor = c; //vec4(d, d, d, 1.);
}
