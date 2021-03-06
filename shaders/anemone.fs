const vec4 outerColor = vec4(0.2, 0.2, 0.8, 0.45);
const vec4 innerColor = vec4(0.8, 0.8, 0.2, 0.2);

const float shadowBias = 0.5;

#define EPSILON 1e-5

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;

in vec3 p0w, p1w;
in float radius;
in vec3 Pw, Nw;

out vec4 color;

// XXX use pcf
float shadow_lookup(in vec3 P, in vec3 N)
{
        vec4 shadowCoord = shadowMatrix * vec4(P + shadowBias * N, 1.);
        shadowCoord /= shadowCoord.w;

        float shadowLookupDepth = texture2D(shadowMap, shadowCoord.st).r;

        return shadowCoord.z < shadowLookupDepth ? 0.25 : 1.;
}

vec4 shade_point(
    in vec3 P,      // point to shade
    in vec3 N,      // normal at point
    in vec3 V,      // view vector
    in vec4 Cs)  // surface color
{
    vec3 L = normalize(lightPos - P);
    float NdotL = clamp(dot(N, L), 0., 1.);
    float shadow = 1.; //shadow_lookup(P, N);

    // shadowed diffuse + ambient for now
    return vec4((NdotL * shadow + 0.1) * Cs.rgb, Cs.a);
}

void main()
{
    vec3 V = normalize(Pw - cameraPos);
    
    // first test intersection with outer tube
    float t1, t2;
    if (!tube_intersect(cameraPos, V, p0w, p1w, radius, t1, t2)) {
        color = vec4(1, 0., 0., 1.);
        //discard;
    } else {
        // XXX assumes t1 < t2 and t2 is always inside hit. does this hold??

        hit hits[MAX_HITS]; // = vec4[4](0.);

        vec3 Poh1 = cameraPos + t1 * V;
        vec3 Noh1 = tube_normal(Poh1, p0w, p1w);

        hits[0].color = shade_point(Poh1, Noh1, V, outerColor);

        vec3 Poh2 = cameraPos + t2 * V;
        vec3 Noh2 = -1. * tube_normal(Poh2, p0w, p1w);

        hits[1].color = shade_point(Poh2, Noh2, V, outerColor);
        
        // now test intersection with inner tube
        if (tube_intersect(cameraPos, V, p0w, p1w, radius / 3., t1, t1)) {
            // move second outer hit to the back of the list to make room
            // for the inner hits
            hits[3].color = hits[1].color;

            vec3 Pih1 = cameraPos + t1 * V;
            vec3 Nih1 = tube_normal(Pih1, p0w, p1w);

            hits[1].color = shade_point(Pih1, Nih1, V, innerColor);

            vec3 Pih2 = cameraPos + t2 * V;
            vec3 Nih2 = -1. * tube_normal(Pih2, p0w, p1w);

            hits[2].color = shade_point(Pih2, Nih2, V, innerColor);

            color = composite_hits(hits, 4);
        } else {
            color = composite_hits(hits, 2);
        }

        color = vec4(Pw, 1.);

        // XXX getting this to work with Phit will be a subtle improvement
        //vec4 PhitProj = projMat * modelViewMat * vec4(Pw, 1.);
        //PhitProj.z /= PhitProj.w;
        //gl_FragDepth = (PhitProj.z + 1.) / 2;
    }

}
