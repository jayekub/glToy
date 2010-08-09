#define PI 3.1415926535897

// Some commonly used PRNG hash function. Where does it come from?
float hash(int n)
{
    n = (n << 13) ^ n;
    n = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
    return float(n);
}

vec3 randVec(int seed, float min, float max)
{
    vec3 v;
    
    v.x = hash(seed * 57 * 113);
    v.y = hash(seed * seed * 113);
    v.z = hash(seed * seed * seed);
    v /= 1073741824.;
    
    v = (max - min) * v + min;
    
    return v;
}

// returns an arbitrary vector perpendicular to V
vec3 perpVec(vec3 V)
{
    // get basis vector corresponding to element of least magnitude
    float minEl = min(V.x, min(V.y, V.z));
    bvec3 whichEl = equal(V, vec3(minEl));
    vec3 basis = 
        whichEl.x ? vec3(1., 0., 0.) : 
            whichEl.y ? vec3(0., 1., 0.) : vec3(0., 0., 1.);
                    
    return normalize(cross(V, basis));
}

vec3 rotateVec(vec3 V, float angle, vec3 axis)
{
    vec3 P = cross(axis, V);
    
    return length(P) == 0 ? normalize(V) :
        normalize(cos(angle) * V + sin(angle) * normalize(P));
}