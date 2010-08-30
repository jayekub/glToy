#define MAX_HITS 4

const float PI = 3.1415926535897;

struct hit {
    //int id;
    float depth;
    vec4 color;
};

/*
// Some commonly used PRNG function. Where does it come from?
float rand(in int n)
{
    n = (n << 13) ^ n;
    n = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
    return float(n);
}
*/

float rand(in vec2 v)
{
    return fract(sin(dot(v.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 randVec(in int seed, in float min, in float max)
{
    vec3 v;
    
    /*
    v.x = hash(seed * 57 * 113);
    v.y = hash(seed * seed * 113);
    v.z = hash(seed * seed * seed);
    v /= 1073741824.;
    */
    
    v.x = rand(seed * vec2(1., 1.));
    v.y = rand((seed + v.x) * vec2(1., 1.));
    v.z = rand((seed + v.y) * vec2(1., 1.));
    
    v = (max - min) * v + min;
    
    return v;
}

// returns an arbitrary vector perpendicular to V
vec3 perpVec(in vec3 V)
{
    // get basis vector corresponding to element of least magnitude
    float minEl = min(V.x, min(V.y, V.z));
    bvec3 whichEl = equal(V, vec3(minEl));
    vec3 basis = 
        whichEl.x ? vec3(1., 0., 0.) : 
            whichEl.y ? vec3(0., 1., 0.) : vec3(0., 0., 1.);
                    
    return normalize(cross(V, basis));
}

vec3 rotateVec(in vec3 V, in float angle, in vec3 axis)
{
    vec3 P = cross(axis, V);
    
    return length(P) == 0 ? normalize(V) :
        normalize(cos(angle) * V + sin(angle) * normalize(P));
}

vec4 ptransform4(in mat4 transMat, in vec3 point)
{
    return transMat * vec4(point, 1.);
}

vec3 ptransform(in mat4 transMat, in vec3 point)
{
    return ptransform4(transMat, point).xyz;
}

vec4 vtransform4(in mat4 transMat, in vec3 vector)
{
    return transMat * vec4(vector, 0.);
}

vec3 vtransform(in mat4 transMat, in vec3 vector)
{
    return vtransform4(transMat, vector).xyz;
}

void sort_hits(inout hit hits[MAX_HITS],
               in int numHits)
{
    // XXX O(n^2) insertion sort
    for (int i = 1; i < numHits - 1; ++i) {
        hit tmp = hits[i];
        int j = i - 1;
        
        while(tmp.depth < hits[j].depth && j >= 0) {
            hits[j + 1] = hits[j];
            --j;
        }
        
        hits[j + 1] = tmp;
    }
}
               
// expects that hits is already sorted by depth
vec4 composite_hits(in hit hits[MAX_HITS], in int numHits)
{
    vec4 finalColor = vec4(0., 0., 0., 0.);

    for (int i = 0; i < numHits && finalColor.a < 1.; ++i)
        finalColor += (1. - finalColor.a) * hits[i].color;

    return finalColor;
}