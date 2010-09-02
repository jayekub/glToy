// one point per tentacle in
layout(points) in;

// num vertices out is 6 * numSegments * numSides
layout(triangle_strip, max_vertices = 128) out;

//layout(points, max_vertices = 250) out;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform float radius;
uniform int numLat;
uniform int numLong;

in int vertexId[];
in vec3 vertex[];

flat out int bubbleId;
out vec3 Pw, Nw;

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
}

void main()
{
    mat4 modelViewProjMat = projMat * viewMat * modelMat;

    vec3 pos = vertex[0];//randVec(vertexId[0], -1., 1.);
    emit_sphere(pos, radius, 
                numLat, numLong, modelViewProjMat);
}
