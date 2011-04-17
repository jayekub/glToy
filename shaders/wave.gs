layout(points) in;

#ifdef DRAW_POINTS
layout(points, max_vertices = 128) out;
#else
layout(triangle_strip, max_vertices = 128) out;
#endif

const int tubeNumSegs = 1;
const int tubeNumSides = 12; 
const float tubeRadius = .5;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform float time;

in vec3 vertex[];

out vec3 Pw;
out vec3 Nw;

float wavefBm(in vec3 p)
{
    float result = 0;

    result += 0.5000*noise3f(0.5*p);
    result += 0.2500*noise3f(1.*p);

    return result;
}

void drawTube(
    in vec3 Pbottom,
    in float height)
{
    mat4 viewProjMat = projMat*viewMat;

    vec3 Ptop = Pbottom + vec3(0, height, 0);
    vec3 Vaxis = Ptop - Pbottom;
    vec3 Vperp = normalize(perpVec(normalize(Vaxis)));
    vec3 Vseg = Vaxis/tubeNumSegs;

    const float thetaInc = 2.*PI/tubeNumSides;

    // shaft
    for (int n = 0; n < tubeNumSegs; ++n) {
        vec3 p0 = Pbottom + n*Vseg;
        vec3 p1 = p0 + Vseg;

        for (int s = 0; s <= tubeNumSides; ++s) {
            vec3 Vrot = normalize(rotateVec(Vperp, s*thetaInc + PI/4., normalize(Vaxis)));

            // to
            Pw = ptransform(modelMat, p1 + tubeRadius*Vrot);
            Nw = vtransform(modelMat, Vrot); 
            gl_Position = ptransform4(viewProjMat, Pw);
            EmitVertex();

            // bottom
            Pw = ptransform(modelMat, p0 + tubeRadius*Vrot);
            Nw = vtransform(modelMat, Vrot); 
            gl_Position = ptransform4(viewProjMat, Pw);
            EmitVertex();
        }
        EndPrimitive();
    }

    // cap
    vec3 NcapW = vtransform(modelMat, Vaxis);

    for (int s = 0; s <= tubeNumSides; ++s) {
        float t0 = s*thetaInc + PI/4.;
        float t1 = t0 + thetaInc;

        vec3 Vrot0 = normalize(rotateVec(Vperp, t0, normalize(Vaxis)));
        vec3 Vrot1 = normalize(rotateVec(Vperp, t1, normalize(Vaxis)));

        Pw = ptransform(modelMat, Ptop + tubeRadius*Vrot0);
        Nw = NcapW;
        gl_Position = ptransform4(viewProjMat, Pw);
        EmitVertex();

        Pw = ptransform(modelMat, Ptop + tubeRadius*Vrot1);
        Nw = NcapW;
        gl_Position = ptransform4(viewProjMat, Pw);
        EmitVertex();

        Pw = ptransform(modelMat, Ptop);
        Nw = NcapW;
        gl_Position = ptransform4(viewProjMat, Pw);
        EmitVertex();
    }
    EndPrimitive();
}

void main()
{

    vec3 Pnoise = (vertex[0] + 10*time*vec3(2., 1., 1.))/10.;
    float height = 10.*wavefBm(Pnoise);

    vec3 Po = vertex[0] + vec3(0, height, 0);

#ifdef DRAW_POINTS

    gl_Position = projMat * viewMat * modelMat * vec4(Po, 1.);
    EmitVertex();
    EndPrimitive();
#else
    drawTube(vertex[0], height + 6);
#endif
}
