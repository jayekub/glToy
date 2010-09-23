in vec3 centerIn;

out vec3 center;

void main()  {
    center = centerIn;
    gl_PointSize = 15.;
}
