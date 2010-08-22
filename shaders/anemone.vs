uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

in vec3 vertex;

out int vertexId;

void main()  {
    vertexId = gl_VertexID;

//    gl_Position = projMat * viewMat * modelMat * vec4(vertex, 1.);
}
