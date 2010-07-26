uniform float lineWidth;
uniform vec3 cameraPos;
uniform vec4 viewport;
uniform mat4x4 fragToWorld;

varying vec3 vp0, vp1;

float cylinder_intersect(vec4 rayorigin, vec4 raydir)
{
    float A = raydir.x * raydir.x + raydir.z * raydir.z;
    float B = 2. * A;
    float C = A - 0.25;
    
    float desc = B * B - 4. * A * C;
    
    if (desc >= 0.) {
        float t1 = (-B + desc) / (2. * A);
        float t2 = (-B - desc) / (2. * A);
        
        return t1 < t2 ? t1 : t2;
    }
    
    return 9999.;
}

vec4 cylinder_normal(vec4 point)
{
    return normalize(vec4(point.x, 0., point.z, 0.));
}

mat4x4 translation_matrix(vec3 t)
{
    return mat4x4(1., 0., 0., t.x,
                  0., 1., 0., t.y,
                  0., 0., 1., t.z,
                  0., 0., 0., 1.);
}

mat4x4 rotation_matrix(float angle, vec3 axis)
{
    float s = sin(angle);
    float c = cos(angle);
    float omc = 1. - c;

    vec3 An = normalize(axis);
    float x = An.x;
    float y = An.y;
    float z = An.z;

    return mat4x4(
        x * x * omc + c,
        y * x * omc - z * s,
        z * x * omc + y * s,
        0.,

        x * y * omc + z * s,
        y * y * omc + c,
        z * y * omc - x * s,
        0.,

        x * z * omc - y * s,
        y * z * omc + x * s,
        z * z * omc + c,
        0.,

        0., 0., 0., 1.);
}

mat4x4 scale_matrix(vec3 s)
{
    return mat4x4(s.x, 0., 0., 0.,
                  0., s.y, 0., 0.,
                  0., 0., s.z, 0.,
                  0., 0., 0., 1.);
}

void main()
{
//    gl_FragColor = vec4(vp0, 1.);

    vec2 screenCoord = 
        (vec2(gl_FragCoord.x, gl_FragCoord.y) - viewport.xy) * viewport.zw;
    vec4 worldCoord = vec4(screenCoord, gl_FragDepth, 1.0) * fragToWorld;
    worldCoord /= worldCoord.w;
    
    vec3 Vseg = vp1 - vp0;
    float segLength = length(Vseg);
    
    Vseg = normalize(Vseg);
    
    float angle = acos(dot(Vseg, vec3(0., 0., 1.)));
    vec3 axis = normalize(cross(Vseg, vec3(0., 0., 1.)));
    
    mat4x4 T = translation_matrix(-vp0);
    mat4x4 R = rotation_matrix(angle, axis);
    mat4x4 S = scale_matrix(vec3(1. / lineWidth, 1. / lineWidth,
                                          1. / segLength));
    
    mat4x4 worldToSeg =  S * R * T;               
                                         
    mat4x4 segToWorld = inverse(worldToSeg);
    
    vec4 Pseg = worldCoord * worldToSeg;
  
  /*  
    vec4 V = normalize(worldCoord - vec4(cameraPos, 1.));
    
    vec4 Oray = vec4(cameraPos, 1.) * worldToSeg;
    vec4 Dray = normalize(V * worldToSeg);

    float t = cylinder_intersect(Oray, Dray);
    vec4 Phit = Oray + t * Dray;
    
    vec4 Pw = Phit * segToWorld;
    vec4 Nw = cylinder_normal(Phit) * segToWorld;
*/
    //gl_FragColor = vec4(dot(Nw, V) * abs(worldCoord.xyz), 1.);
    gl_FragColor = vec4(Pseg.z, Pseg.z, Pseg.z, 1.);
}