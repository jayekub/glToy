layout(points) in;
//layout(points, max_vertices = 60) out;
//layout(line_strip, max_vertices = 60) out;
layout(triangle_strip, max_vertices = 60) out;

uniform float time;
uniform int tubeNumSegments;
uniform int tubeNumSides;

in int vertexId[];

out vec3 p0, p1;
out vec3 N;
out float radius;

void main()
{
    vec3 tubeDir = randVec(vertexId[0] + 1, -1., 1.);
    
    //tubeDir = rotateVec(tubeDir, mod(time, 2. * PI), vec3(0., 1., 0.));
    
    vec3 currentDir = vec3(1., 1., 0.);
    vec3 timeOffset = time * vec3(1., 1., 1.) / 4.;
    
    vec3 tubePts[5];
    for (int s = 0; s < tubeNumSegments + 1; ++s) {
        tubePts[s] = 
            (float(s) / float(tubeNumSegments)) * tubeDir + 
            0.05 * randVec((vertexId[0] + 1) * s, -1., 1.);
            
        tubePts[s] += 0.15 * snoise(tubePts[s] + timeOffset) * currentDir;
            
        //gl_Position = vec4(tubePts[s], 1.);
        //EmitVertex();
    }
    //EndPrimitive();
    
    // line vertices
    float botRad, topRad;
     
    botRad = 0.02;
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
            N = tube_normal(gl_Position.xyz, p0, p1);
            
            EmitVertex(); 
            //EndPrimitive();
            
            radius = topRad;
            gl_Position = vec4(p1 + radius * Prot, 1.);
            N = tube_normal(gl_Position.xyz, p0, p1);
            
            EmitVertex();
            //EndPrimitive();
        }
    }
    EndPrimitive();
}