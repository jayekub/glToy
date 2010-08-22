bool sphere_intersect(    
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
    
    if (desc >= 0.) {
        desc = sqrt(desc);
    
        t1 = (-B + desc) / (2. * A);
        t2 = (-B - desc) / (2. * A);

        return true;
    } else {
        return false;
    }
}

vec3 sphere_normal(
    in vec3 T,      // point on sphere 
    in vec3 P)      // sphere position
{
    return normalize(T - P);
}