#version 120

uniform vec3 cameraPos;
uniform vec4 viewport;
uniform mat4 fragToWorld;
uniform float lineWidth;

varying vec3 fp0, fp1;

// from http://www.gamedev.net/community/forums/topic.asp?topic_id=467789
//
// Ray R(t) = O + V*t
// Cylinder from points P to Q with radius r
//
// R(t) is on the cylinder if |(R(t) - P) x (Q - P)|^2 = r^2 * |Q - P|^2
// (note: |Q - P|^2 term is to normalize axis)
//
// Then (X + t * Y)^2 = d where
// 
//     X = (O - P) x (Q - P)
//     Y = V x (Q - P)
//     d = r^2 * |Q - P|^2
//
// and t^2 * (Y . Y) + 2*t*(X . Y) + (X . X) - d = 0
//
// This function solves for t.

float cylinder_intersect(vec3 O, vec3 V, vec3 P, vec3 Q, float r)
{
    vec3 PO = O - P;
    vec3 PQ = Q - P;
    float pq2 = dot(PQ, PQ);
    
    vec3 X = cross(PO, PQ);
    vec3 Y = cross(V, PQ);
    float d = r * r * pq2;
    
    float A = dot(Y, Y);
    float B = 2 * dot(X, Y);
    float C = dot(X, X) - d;
    
    float desc = B * B - 4. * A * C;
    
    if (desc >= 0.) {
        float t1 = (-B + desc) / (2. * A);
        float t2 = (-B - desc) / (2. * A);
        
        return t1 < t2 ? t1 : t2;
    } else {
        return -9999.;
    }
}

// Returns the normal at point T on the cylinder with end points P and Q
vec3 cylinder_normal(vec3 T, vec3 P, vec3 Q)
{
    vec3 PQ = Q - P;
    vec3 PT = T - P;

    // XXX something's wrong here...
    return normalize(cross(cross(PQ, PT), PQ));
}

void main()
{
    vec2 screenCoord = 
        (vec2(gl_FragCoord.x, gl_FragCoord.y) - viewport.xy) * viewport.zw;
    vec4 worldCoord = fragToWorld * vec4(screenCoord, gl_FragCoord.z, 1.0);
    worldCoord /= worldCoord.w;

    vec3 V = normalize(worldCoord.xyz - cameraPos);
    float t = cylinder_intersect(cameraPos, V, fp0, fp1, lineWidth);
    
    if (t < 0) {
        gl_FragColor = vec4(1, 0., 0., 1.);
        //discard;
    } else {
    
        vec3 Phit = cameraPos + t * V;
        vec3 N = cylinder_normal(Phit, fp0, fp1);
        float NdotL = clamp(dot(N, vec3(-1., 0., 0.)), 0., 1.);

    //gl_FragColor = vec4(NdotL * NdotL, NdotL, NdotL + 0.1, 1.);
    //gl_FragDepth = (Phit.z - gl_DepthRange.near) / 5.;
    
       gl_FragColor = vec4(abs(N).xyz, 1.);
    }  
 }