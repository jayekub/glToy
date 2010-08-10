uniform mat4 modelViewMat;
uniform mat4 projMat;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;

in vec3 p0, p1;
in float radius;
in vec3 Pw, Nw;

out vec4 color;

void main()
{
    vec3 V = normalize(Pw - cameraPos);

    float t1, t2;
    if (!tube_intersect(cameraPos, V, p0, p1, radius, t1, t2) ||
        t1 < 0. || t2 < 0.) {
        
        color = vec4(1, 0., 0., 1.);
        //discard;
    } else {
        float t = t1 < t2 ? t1 : t2;
    
        vec3 Phit = cameraPos + t * V;
        vec3 N = tube_normal(Phit, p0, p1);
        vec3 L = normalize(lightPos - Phit);
       
        // XXX getting this to work with Phit will be a subtle improvement
        vec4 PhitProj = projMat * modelViewMat * vec4(Pw, 1.);
        PhitProj.z /= PhitProj.w;
        
        gl_FragDepth = (PhitProj.z + 1.) / 2;
        
        
        vec4 shadowCoord = shadowMatrix * vec4(Phit + 3. * N, 1.);
        shadowCoord /= shadowCoord.w;
        
        float shadowLookupDepth = texture2D(shadowMap, shadowCoord.st).r;
        float shadowAtten = shadowCoord.z < shadowLookupDepth ? 0.25 : 1.;
        
        float NdotL = clamp(dot(N, L), 0.1, 1.);
        
        color = vec4(NdotL, NdotL, NdotL, 1.);

        //float NdotE = dot(N, -V);
        //float OmNdotE = 1. - NdotE;
       
        // lighting + fog
        /*float intensity = NdotL * shadowAtten;
        float omi = 1. - intensity;
    
        vec4 baseColor = mix(vec4(1., 1., 0.56, 1.), vec4(.8, .8, .6, 1.),
                             omi * omi);
        vec4 fogColor = mix(vec4(0.03, .0, .07, 1.),
                            vec4(0.08, 0.05, 0.06, 1.), sqrt(intensity));
           
        //gl_FragColor = mix(fogColor, baseColor, intensity);*/
    }
}