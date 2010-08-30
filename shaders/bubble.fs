const vec4 bubbleColor = vec4(0.2, 0.2, 0.8, 0.45);

uniform mat4 modelMat;

uniform vec3 cameraPos;
uniform vec3 lightPos;

uniform float radius;

uniform int numBubbles;

in int bubbleId;
in vec3 Pw, Nw;

out vec4 color;

vec4 shade_point(
    in vec3 P,      // point to shade
    in vec3 N,      // normal at point
    in vec3 V,      // view vector
    in vec4 Cs)     // surface color
{
    vec3 L = normalize(lightPos - P);
    float NdotL = clamp(dot(N, L), 0., 1.);
    float shadow = 1.; //shadow_lookup(P, N);

    // shadowed diffuse + ambient for now
    return vec4((NdotL * shadow + 0.1) * Cs.rgb, Cs.a);
}

bool trace_bubble(
    in int bubbleId,
    in vec3 O,
    in vec3 V,
    out hit hit1,
    out hit hit2)
{
    vec3 center = ptransform(modelMat, randVec(bubbleId, -1., 1.));
    
    float t1, t2;
    if (sphere_intersect(O, V, center, radius, t1, t2)) {
        vec3 Phit1 = O + t1 * V;
        vec3 Nhit1 = sphere_normal(Phit1, center);
        
        //hit1.id = bubbleId;
        hit1.depth = t1;
        hit1.color = vec4(1., 0, 0, 0.5); //shade_point(Phit1, Nhit1, V, bubbleColor);
        
        vec3 Phit2 = O + t2 * V;
        vec3 Nhit2 = -1. * sphere_normal(Phit2, center);
        
        //hit2.id = bubbleId;
        hit2.depth = t2;
        hit2.color = vec4(0, 0, 1, 0.5); //shade_point(Phit2, Nhit2, V, bubbleColor);
        
        return true;
    } else {
        return false;
    }
}

void main()
{
    int numHits = 0;
    hit hits[MAX_HITS];

    vec3 V = normalize(Pw - cameraPos);
    
    if (trace_bubble(bubbleId, cameraPos, V, hits[numHits], hits[numHits + 1])) {
        numHits += 2;
    }
    
    for (int i = 0; i < 1 && numHits < MAX_HITS - 1; ++i) {
        if (i != bubbleId &&
            trace_bubble(0, cameraPos, V, hits[numHits], hits[numHits + 1])) {
            numHits += 2;
        }
    }
    
    color = composite_hits(hits, numHits);
    
    /*
    vec3 center = ptransform(modelMat, randVec(bubbleId, -1., 1.));

    float t1, t2;

    if (!sphere_intersect(cameraPos, V, center, radius, t1, t2)) {
        color = vec4(1., 0., 0., 1.);
    } else {
        vec3 Phit1 = cameraPos + t1 * V;
        vec3 Nhit1 = sphere_normal(Phit1, center);
    
        float NdotV = clamp(dot(Nhit1, V), 0.1, 1.);
    
        color = vec4(NdotV, NdotV, NdotV, 1.);
    }*/
}
