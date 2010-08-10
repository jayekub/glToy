uniform mat4 modelViewMat;
uniform mat4 projMat;

in vec3 vertex;

out int vertexId;

void main()  {
    vertexId = gl_VertexID;

    gl_Position = projMat * modelViewMat * vec4(vertex, 1.);
}
