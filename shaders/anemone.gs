// one point per tentacle in
layout(points) in;

// num vertices out is 6 * numSegments * numSides
layout(triangle_strip, max_vertices = 60) out;

const float startRadius = 0.03;
const float wiggle = 0.05;

uniform float time;
uniform int tubeNumSegments;
uniform int tubeNumSides;

in int vertexId[];

out vec3 p0, p1;
out vec3 Pw, Nw;
out float radius;

void main()
{
    vec3 tubeDir = randVec(vertexId[0] + 1, -1., 1.);
    
    //tubeDir = rotateVec(tubeDir, mod(time, 2. * PI), vec3(0., 1., 0.));
    
    vec3 currentDir = vec3(1., 1., 0.);
    
    vec3 tubePts[5];
    for (int s = 0; s < tubeNumSegments + 1; ++s) {
        tubePts[s] = 
            (float(s) / float(tubeNumSegments)) * tubeDir + 
            wiggle * randVec((vertexId[0] + 1) * s, -1., 1.);
            
        tubePts[s] += 0.3 * noise(vec4(tubePts[s], time / 2.)) * currentDir;

        //gl_Position = vec4(tubePts[s], 1.);
        //EmitVertex();
    }
    //EndPrimitive();
    
    // line vertices
    float botRad, topRad;
     
    botRad = startRadius;
    for (int n = 1; n < tubeNumSegments + 1; ++n) {
        p0 = tubePts[n - 1];
        p1 = tubePts[n];
        
        topRad = botRad; // XXX
          
        vec3 tubeAxis = p1 - p0;
        vec3 Perp = normalize(perpVec(normalize(tubeAxis)));

        for (int s = 0; s <= tubeNumSides; ++s) {
            float theta = 2. * PI * s / tubeNumSides;
            vec3 Prot = normalize(rotateVec(Perp, theta, normalize(tubeAxis)));
            
            radius = botRad;
            gl_Position = vec4(p0 + radius * Prot, 1.);
            
            Pw = gl_Position.xyz;
            Nw = tube_normal(Pw, p0, p1);
            
            EmitVertex(); 
            //EndPrimitive();
            
            radius = topRad;
            gl_Position = vec4(p1 + radius * Prot, 1.);
            
            Pw = gl_Position.xyz;
            Nw = tube_normal(Pw, p0, p1);
            
            EmitVertex();
            //EndPrimitive();
        }
    }
    EndPrimitive();
}