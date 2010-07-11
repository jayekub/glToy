uniform sampler2D tex0;
uniform sampler2D tex1;

void main() {
    vec4 mixed = mix(texture2D(tex0, gl_TexCoord[0].st),
                     texture2D(tex1, 3.0 * gl_TexCoord[0].st), 0.25);
                       
    gl_FragColor = vec4(mixed.rgb, 1.0);
}