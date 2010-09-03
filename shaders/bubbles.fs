uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform vec3 cameraPos;
uniform vec3 lightPos;

uniform float radius;

uniform int numBubbles;
uniform samplerBuffer bubbleCenters;

flat in int bubbleId;
in vec3 Pw, Nw;

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

    return vec4((ambient + diffuse + specular)* shadow * Cs.rgb, Cs.a);
}

int trace_bubble(
    in int bubbleId,
    in vec3 O,
    in vec3 V,
    out hit hit1,
    out hit hit2)
{
    int index = 3 * bubbleId;
    vec3 center =
        ptransform(modelMat,
                    vec3(texelFetch(bubbleCenters, index + 0).r,
                         texelFetch(bubbleCenters, index + 1).r,
                         texelFetch(bubbleCenters, index + 2).r));

    vec4 bubbleColors[3];

    bubbleColors[0] = vec4(0.5, 0.5, 0.9, 0.35);
    bubbleColors[1] = vec4(0.9, 0.5, 0.5, 0.35);
    bubbleColors[2] = vec4(0.5, 0.9, 0.5, 0.35);

    int colorNum = int(floor(3 * rand(bubbleId * vec2(1.))));
    vec4 bubbleColor = bubbleColors[colorNum];

    float t1, t2;
    int numHit = sphere_intersect(O, V, center, 3.75*radius, t1, t2);

    // XXX will be garbage if numHit is zero

    vec3 Phit1 = O + t1 * V;
    vec3 Nhit1 = sphere_normal(Phit1, center);

    //hit1.id = bubbleId;
    hit1.depth = t1;
    hit1.color = shade_point(Phit1, Nhit1, V, bubbleColor);

    vec3 Phit2 = O + t2 * V;
    vec3 Nhit2 = -1. * sphere_normal(Phit2, center);

    //hit2.id = bubbleId;
    hit2.depth = t2;
    hit2.color = shade_point(Phit2, Nhit2, V, bubbleColor);

    return numHit;
}

void main()
{
    int numHits = 0;
    hit hits[MAX_HITS];

    vec3 V = normalize(Pw - cameraPos);

    // XXX will fail if more than MAX_HITS / 2 bubbles overlap
    for (int i = 0; i < numBubbles && numHits < MAX_HITS - 1; ++i) {
        numHits += trace_bubble(i, cameraPos, V,
                                hits[numHits], hits[numHits + 1]);
    }

    sort_hits(hits, numHits);
    color = composite_hits(hits, numHits);

    /*
    vec4 Pclosest = ptransform4(projMat * viewMat * modelMat,
                                cameraPos + hits[0].depth * V);
    Pclosest.z /= Pclosest.w;
    gl_FragDepth = (Pclosest.z + 1.) / 2.;
    */
}
