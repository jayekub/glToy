uniform vec3 cameraPos;

in vec3 Pw;
in vec3 Nw;

out vec4 color;

void main()
{
    vec3 L = normalize(cameraPos - Pw);
    color = vec4(dot(normalize(Nw), L)*vec3(.6, .6, .8), 1.);
}
