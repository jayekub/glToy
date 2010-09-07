// one point per particle in
layout(points) in;

// max vertices out is 2 * numSides
layout(triangle_strip, max_vertices = 128) out;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec3 cameraPos;

in vec3 center[];
in float radius[];

out vec3 Pw;

flat out vec3 bcenter;
flat out float bradius;

void emit_regpoly(
    in vec3 Cw,             // polygon center
    in vec3 Nw,             // polygon normal
    in float radiusW,       // radius of bubble
    in float circumradiusW, // radius of polygon
    in int sides,           // number of sides
    in mat4 viewProjMat)    // projection*view matrix
{
    float innerAngle = 2. * PI / float(sides);

    vec4 Cs = ptransform4(viewProjMat, Cw);
    vec3 Vw = normalize(perpVec(Nw));
    
    // XXX only need to emit single vertex for last iteration
    for (int p = 0; p <= sides; ++p) {
        float phi = p * innerAngle;
        vec3 Rw = normalize(rotateVec(Vw, phi, Nw));
        
        bcenter = Cw;
        bradius = radiusW;
        Pw = Cw;
        gl_Position = Cs;
            
        EmitVertex();
        
        bcenter = Cw;
        bradius = radiusW;
        Pw = Cw + circumradiusW * Rw;
        gl_Position = ptransform4(viewProjMat, Pw);
            
        EmitVertex(); 
    }
    EndPrimitive();
}

void main()
{
    mat4 viewProjMat = projMat * viewMat;

    const int sides = 4;
    vec3 Cw = ptransform(modelMat, center[0]); 
    vec3 Vw = normalize(cameraPos - Cw);

    float circumradiusW = radius[0] / cos(PI / sides);

    emit_regpoly(Cw, Vw, radius[0], circumradiusW, sides, viewProjMat);
}