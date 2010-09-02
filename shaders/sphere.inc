int sphere_intersect(    
    in vec3 O,      // ray origin
    in vec3 V,      // ray direction
    in vec3 P,     // sphere position
    in float r,     // sphere radius
    out float t1,
    out float t2)
{
    vec3 X = O - P;
    vec3 Y = V;
    float d = r * r;

    float A = dot(Y, Y);
    float B = 2. * dot(X, Y);
    float C = dot(X, X) - d;

    float desc = B * B - 4. * A * C; 

    // XXX avoids branching
    float sqrtdesc = sqrt(desc);

    // XXX try vectorizing this?
    t1 = (-B + sqrtdesc) / (2. * A);
    t2 = (-B - sqrtdesc) / (2. * A);

    return 2 * int(step(0., desc));
}

vec3 sphere_normal(
    in vec3 T,      // point on sphere 
    in vec3 P)      // sphere position
{
    return normalize(T - P);
}
