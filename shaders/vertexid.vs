in vec3 vertexIn;

out int vertexId;
out vec3 vertex;

void main()  {
    vertexId = gl_VertexID;
    vertex = vertexIn;
}
