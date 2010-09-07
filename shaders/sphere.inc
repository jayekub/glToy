int sphere_intersect(    
    in vec3 O,      // ray origin
    in vec3 V,      // ray direction
    in vec3 P,      // sphere position
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


/* may be useful in the future

// XXX optimize by only emitting front-facing triangles?
void emit_sphere(
    in vec3 position,
    in float radius,
    in float numLat,
    in float numLong,
    in mat4 modelViewProjMat)
{
    for (int p = 0; p < numLong; ++p) {
        float phi0 = PI * (float(p) / float(numLong));
        float phi1 = PI * (float(p + 1) / float(numLong));
    
        for (int t = 0; t < numLat; ++t) {
            float theta = 2. * PI * (float(t) / float(numLat));
            
            vec3 P0 = position + 
                      vec3(radius * cos(theta) * sin(phi0),
                           radius * sin(theta) * sin(phi0),
                           radius * cos(phi0));
                             
            bubbleId = vertexId[0];      
            Pw = ptransform(modelMat, P0);
            Nw = vtransform(modelMat, normalize(P0));                 
                                
            gl_Position = ptransform4(modelViewProjMat, P0);
            EmitVertex();
            
            vec3 P1 = position +
                      vec3(radius * cos(theta) * sin(phi1),
                           radius * sin(theta) * sin(phi1),
                           radius * cos(phi1));
                          
            bubbleId = vertexId[0]; 
            Pw = ptransform(modelMat, P1);
            Nw = vtransform(modelMat, normalize(P1));                      
                                
            gl_Position = ptransform4(modelViewProjMat, P1);
            EmitVertex();
        }
    }
    EndPrimitive();
}
*/