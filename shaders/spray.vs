uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

in vec3 centerIn;
in float radiusIn;

out vec3 center;
out float radius;

void main()  {
    center = centerIn;
    radius = radiusIn;

    vec3 Cw = ptransform(viewMat * modelMat, centerIn);
    gl_PointSize = 10. * radiusIn / -Cw.z;

    gl_Position = ptransform4(projMat * viewMat * modelMat, centerIn);
}
