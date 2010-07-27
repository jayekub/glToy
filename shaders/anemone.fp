#version 150

uniform vec3 cameraPos;
uniform vec4 viewport;
uniform mat4 fragToWorld;
uniform float lineWidth;

varying vec3 vp0, vp1;

// from http://www.gamedev.net/community/forums/topic.asp?topic_id=467789
float cylinder_intersect(vec3 O, vec3 V, vec3 a, vec3 b, float r)
{
    vec3 ab = b - a;
    vec3 aO = O - a;
    vec3 aOxab = cross(aO, ab);
    vec3 Vxab = cross(V.xyz, ab);
    float ab2 = dot(ab, ab);
    
    float A = dot(Vxab, Vxab);
    float B = 2. * dot(Vxab, aOxab);
    float C = dot(aOxab, aOxab) - (r * r * ab2);
    
    float desc = B * B - 4. * A * C;
    
    if (desc >= 0.) {
        float t1 = (-B + desc) / (2. * A);
        float t2 = (-B - desc) / (2. * A);
        
        return t1 < t2 ? t1 : t2;
    } else {
        return -9999.;
    }
}

vec3 cylinder_normal(vec3 P, vec3 A, vec3 B)
{
    vec3 AB = B - A;
    vec3 AP = P - A;

    return normalize(cross(cross(AB, AP), AB));
       
    /*
    vec3 Pproj = dot(AP, normalize(AB)) * AB + A;

    return normalize(P - Pproj);*/
}

void main()
{
//    gl_FragColor = vec4(vp0, 1.);

    vec2 screenCoord = 
        (vec2(gl_FragCoord.x, gl_FragCoord.y) - viewport.xy) * viewport.zw;
    vec4 worldCoord = fragToWorld * vec4(screenCoord, gl_FragCoord.z, 1.0);
    worldCoord /= worldCoord.w;
    
    vec3 Ow = cameraPos;
    vec3 Vw = normalize(worldCoord.xyz - cameraPos);

    float t = cylinder_intersect(Ow, Vw, vp0, vp1, lineWidth);
    
    //if (t < 0) discard;
    
    if (t < 0) {
        gl_FragColor = vec4(1, 0., 0., 1.);
    } else {
    
    vec3 Phit = Ow + t * Vw;
    
    //vec4 Pw = vec4(Phit, 1.) * segToWorld;
    vec3 Nw = cylinder_normal(Phit, vp0, vp1);

    float NdotL = clamp(dot(Nw, vec3(0., 1., 0)), 0., 1.);

    gl_FragColor = vec4(NdotL, NdotL, NdotL + 0.1, 1.);
    gl_FragDepth = (Phit.z - gl_DepthRange.near) / 5.;
    
    //gl_FragColor = vec4(Vw.xyz, 1.);
    }
}