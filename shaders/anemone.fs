uniform float viewportWidth;
uniform float viewportHeight;
uniform mat4 fragToWorld;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform sampler2D shadowMap;
uniform mat4 shadowMatrix;

in vec3 p0, p1;
in float radius;

in vec4 color;
in vec3 N;

void main()
{
    gl_FragColor = vec4(1., 0., 0., 1.);

#if 0
    vec4 viewport = vec4(0., 0., 1. / viewportWidth, 1. / viewportHeight);
    vec2 screenCoord = 
        (vec2(gl_FragCoord.x, gl_FragCoord.y) - viewport.xy) * viewport.zw;
    vec4 worldCoord = fragToWorld * vec4(screenCoord, gl_FragCoord.z, 1.0);
    worldCoord /= worldCoord.w;

    vec3 V = worldCoord.xyz - cameraPos;
    float worldDist = length(V);

    //V = normalize(V);

    float t = tube_intersect(cameraPos, V, p0, p1, 100.);
    
    if (t < 0) {
        gl_FragColor = vec4(1, 0., 1., 1.);
        //discard;
    } else {
        vec3 Phit = cameraPos + t * V;
        vec3 N = tube_normal(Phit, p0, p1);
        vec3 L = lightPos - worldCoord.xyz;
       
        gl_FragColor = vec4(N.xyz, 1.);
       
        vec4 PhitProj = gl_ModelViewProjectionMatrix * vec4(Phit, 1.);
        PhitProj.z /= PhitProj.w;
        
        float hitDepth = (PhitProj.z + 1.) / 2;
        
        vec4 shadowCoord = shadowMatrix * vec4(Phit + 3. * N, 1.);
        shadowCoord /= shadowCoord.w;
        
        float shadowLookupDepth = texture2D(shadowMap, shadowCoord.st).r;
        float shadowAtten = shadowCoord.z < shadowLookupDepth ? 0.25 : 1.;
        
        float NdotL = clamp(dot(N, L), 0.1, 1.);
        
        //float NdotE = dot(N, -V);
        //float OmNdotE = 1. - NdotE;
       
        // lighting + fog
        float intensity = NdotL + 0.01 * shadowAtten;
        float omi = 1. - intensity;
    
        /* vec4 baseColor = mix(vec4(1., 1., 0.56, 1.), vec4(.8, .8, .6, 1.),
                             omi * omi);
        vec4 fogColor = mix(vec4(0.03, .0, .07, 1.),
                            vec4(0.08, 0.05, 0.06, 1.), sqrt(intensity));
           
        //gl_FragColor = mix(fogColor, baseColor, intensity);*/
        
        //gl_FragColor = vec4(0.5, 0.5, 1., 1.) * intensity;
        
        //gl_FragDepth = hitDepth;

    }
#endif
}
