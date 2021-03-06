const vec4 bubbleColor = vec4(0.4, 0.4, 0.7, 0.3);

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform vec3 cameraPos;
uniform vec3 lightPos;

flat in vec3 pcenter;
flat in float pradius;
in vec3 Pw;
in vec3 Vw;

out vec4 color;

vec4 shade_point(
    in vec3 P,      // point to shade
    in vec3 N,      // normal at point
    in vec3 V,      // view vector
    in vec4 Cs)     // surface color
{
    vec3 L = normalize(lightPos - P);
    vec3 R = normalize(reflect(V, N));

    float NdotL = clamp(dot(N, L), 0., 1.);

    float shadow = 1.; //shadow_lookup(P, N);

    // ambient + diffuse + spec for now
    float ambient = 0.1;
    float diffuse = NdotL;
    float specular = 2. * pow(max(0., dot(L, R)), 5.);

    return vec4((ambient + diffuse + specular) * shadow * Cs.rgb, Cs.a);
}

void main()
{
//    color = vec4(pcenter.xyz, 1.);

#if 1
    // persp
    vec3 O = cameraPos;
    vec3 V = normalize(Pw - cameraPos);

    // ortho 
//    vec3 O = vec3(Pw.x, Pw.y, cameraPos.z);
//    vec3 V = normalize(vec3(0., 0., Pw.z - cameraPos.z));

    float t1, t2;
    if (sphere_intersect(O, V, pcenter, pradius, t1, t2) < 2) {
//        color = vec4(0., 0., 1., 1.);
        discard;
    } else {
//        color = vec4(.2, .2, .6, .5);

        vec3 Phit1 = O + t1 * V;
        vec3 Nhit1 = sphere_normal(Phit1, pcenter);

        vec4 color1 = shade_point(Phit1, Nhit1, V, bubbleColor);

        vec3 Phit2 = O + t2 * V;
        vec3 Nhit2 = -1. * sphere_normal(Phit2, pcenter);

        vec4 color2 = shade_point(Phit2, Nhit2, V, bubbleColor);

        color = color1 + (1. - color1.a) * color2;

/*
        vec4 PclosestC = ptransform4(projMat * viewMat, Phit1);
        PclosestC.z /= PclosestC.w;

        gl_FragDepth = (PclosestC.z + 1.) / 2.;
*/
    }
#endif
}
