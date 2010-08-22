uniform mat4 modelMat;
uniform vec3 cameraPos;
uniform float radius;

in vec3 Pw, Nw;

out vec4 color;

void main()
{
    vec3 V = normalize(Pw - cameraPos);
    vec3 center = (modelMat * vec4(0., 0., 0., 1)).xyz;

    float t1, t2;

    if (!sphere_intersect(cameraPos, V, center, radius, t1, t2)) {
        color = vec4(1., 0., 0., 1.);
    } else {
        vec3 Phit1 = cameraPos + t1 * V;
        vec3 Nhit1 = sphere_normal(Phit1, center);
    
        float NdotV = clamp(dot(Nhit1, V), 0.1, 1.);
    
        color = vec4(NdotV, NdotV, NdotV, 1.);
    }
}
