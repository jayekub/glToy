// one point per tentacle in
layout(points) in;

// num vertices out is 6 * numSegments * numSides
layout(triangle_strip, max_vertices = 200) out;

//layout(points, max_vertices = 250) out;

const float PI = 3.1415926535;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform float radius;
uniform int numLat;
uniform int numLong;

out vec3 Pw, Nw;

void main()
{
    mat4 modelViewProjMat = projMat * viewMat * modelMat;

    for (int p = 0; p < numLong; ++p) {
        float phi0 = PI * (float(p) / float(numLong));
        float phi1 = PI * (float(p + 1) / float(numLong));
    
        for (int t = 0; t < numLat; ++t) {
            float theta = 2. * PI * (float(t) / float(numLat));
            
            vec4 P0 = vec4(radius * cos(theta) * sin(phi0),
                           radius * sin(theta) * sin(phi0),
                           radius * cos(phi0), 1.);
                                   
            Pw = (modelMat * P0).xyz;
            Nw = (modelMat * vec4(normalize(P0).xyz, 0.)).xyz;                      
                                
            gl_Position = modelViewProjMat * P0;
            EmitVertex();
            
            vec4 P1 = vec4(radius * cos(theta) * sin(phi1),
                           radius * sin(theta) * sin(phi1),
                           radius * cos(phi1), 1.);
                           
            Pw = (modelMat * P1).xyz;
            Nw = (modelMat * vec4(normalize(P1).xyz, 0.)).xyz;                      
                                
            gl_Position = modelViewProjMat * P1;
            EmitVertex();
        }
    }
}