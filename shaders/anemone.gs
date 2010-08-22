// one point per tentacle in
layout(points) in;

// num vertices out is 6 * numSegments * numSides
layout(triangle_strip, max_vertices = 200) out;

//layout(points, max_vertices = 60) out;

const float startRadius = 0.03;
const float wiggle = 0.05;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform float time;
uniform int tubeNumSegments;
uniform int tubeNumSides;

in int vertexId[];

out vec3 p0w, p1w;
out vec3 Pw, Nw;
out float radius;

void main()
{
    mat4 modelViewProjMat = projMat * viewMat * modelMat;

    vec3 tubeDir = randVec(vertexId[0] + 1, -1., 1.);

    vec3 currentDir = vec3(1., 1., 0.);
    
    // compute tube points in object space
    vec3 tubePts[5];
    for (int s = 0; s < tubeNumSegments + 1; ++s) {
        tubePts[s] = 
            (float(s) / float(tubeNumSegments)) * tubeDir + 
            wiggle * randVec((vertexId[0] + 1) * s, -1., 1.);
            
        tubePts[s] += 0.3 * noise(vec4(tubePts[s], time / 2.)) * currentDir;

//        gl_Position = vec4(tubePts[s], 1.) * modelViewProjMat; 
//        EmitVertex();
//        EndPrimitive();
    }
   
#if 1
    // line vertices
    float botRad, topRad;
     
    botRad = startRadius;
    for (int n = 1; n < tubeNumSegments + 1; ++n) {
        vec3 p0 = tubePts[n - 1];
        vec3 p1 = tubePts[n];
        
        p0w = (modelMat * vec4(p0, 1.)).xyz;
        p1w = (modelMat * vec4(p1, 1.)).xyz;
        
        topRad = botRad; // XXX
          
        vec3 tubeAxis = p1 - p0;
        vec3 Perp = normalize(perpVec(normalize(tubeAxis)));

        for (int s = 0; s <= tubeNumSides; ++s) {
            float theta = 2. * PI * s / tubeNumSides;
            vec3 Rrot = normalize(rotateVec(Perp, theta, normalize(tubeAxis)));
            
            vec4 Ptop = vec4(p1 + topRad * Rrot, 1.);

            radius = topRad;
            Pw = (modelMat * Ptop).xyz;
            Nw = tube_normal(Pw, p0w, p1w);
            
            gl_Position = modelViewProjMat * Ptop;
            
            EmitVertex();
            //EndPrimitive();
            
            // bottom point in object space
            vec4 Pbot = vec4(p0 + botRad * Rrot, 1.);
            
            radius = botRad;
            Pw = (modelMat * Pbot).xyz;
            Nw = tube_normal(Pw, p0w, p1w);
            
            gl_Position = modelViewProjMat * Pbot;
            
            EmitVertex(); 
            //EndPrimitive();
        }
        EndPrimitive();
    }
    EndPrimitive();
#endif
}