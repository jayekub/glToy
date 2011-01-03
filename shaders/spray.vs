uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

//in vec3 centerIn;
//in float radiusIn;

attribute vec3 centerIn;
attribute vec3 velocityIn;
attribute float radiusIn;

//out vec3 center;
//out float radius;

//varying vec3 center;
varying float radius;
//varying vec3 velocity;

void main()  {
    //center = centerIn;
    radius = radiusIn;
    //velocity = velocityIn;

    vec3 Cw = ptransform(viewMat * modelMat, centerIn);
    gl_PointSize = 10. * radiusIn / -Cw.z;

    gl_Position = ptransform4(projMat * viewMat * modelMat, centerIn);
}
