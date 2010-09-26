uniform sampler2D tex0;
uniform sampler2D tex1;

in vec2 texcoord;

out vec4 color;

void main() {
    vec4 col0 = texture(tex0, texcoord);
    vec4 col1 = texture(tex1, texcoord);

    color = vec4(col0.r, col1.g, col0.b + col1.b, 1.0);
}
