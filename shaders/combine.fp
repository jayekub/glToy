uniform sampler2D tex0;
uniform sampler2D tex1;

void main() {
    vec4 col0 = texture2D(tex0, gl_TexCoord[0].st);
    vec4 col1 = texture2D(tex1, gl_TexCoord[0].st);

    vec4 mixed = mix(col0, col1, 0.3);
                       
//    gl_FragColor = vec4(mixed.rgb, 1.0);

    gl_FragColor = vec4(col0.r, col1.g, col0.b + col1.b, 1.0);
    
//    gl_FragColor = texture2D(tex0, gl_TexCoord[0].st);
}