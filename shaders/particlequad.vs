in vec3 centerIn;
in vec3 velocityIn;
in float radiusIn;

out vec3 center;
out vec3 velocity;
out float radius;

void main()  {
    center = centerIn;
    velocity = velocityIn;
    radius = radiusIn;
}
