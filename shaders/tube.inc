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
// This function solves for t and returns true if there's at least
// one solution.
//
// XXX still isn't always correct...wtf??

bool tube_intersect(
    in vec3 O,      // ray origin
    in vec3 V,      // ray direction
    in vec3 P,      // cylinder axis point 1
    in vec3 Q,      // cylinder axis point 2
    in float r,     // cylinder radius
    out float t1,   // intersection parameter 1
    out float t2)   // intersection parameter 2
{
    vec3 PO = O - P;
    vec3 PQ = Q - P;
    float pq2 = dot(PQ, PQ);
    
    vec3 X = cross(PO, PQ);
    vec3 Y = cross(V, PQ);
    float d = r * r * pq2;
    
    float A = dot(Y, Y);
    float B = 2. * dot(X, Y);
    float C = dot(X, X) - d;
    
    float desc = B * B - 4. * A * C;
    
    if (desc >= 0.) {
        desc = sqrt(desc);
    
        t1 = (-B + desc) / (2. * A);
        t2 = (-B - desc) / (2. * A);

        return true;
    } else {
        return false;
    }
}

// Returns the normal at point T on the cylinder with end points P and Q
vec3 tube_normal(
    in vec3 T,      // point on cylinder 
    in vec3 P,      // cylinder axis point 1
    in vec3 Q)      // cylinder axis point 2
{
    vec3 PQ = normalize(Q - P);
    vec3 PT = normalize(T - P);

    return normalize(cross(cross(PQ, PT), PQ));
}