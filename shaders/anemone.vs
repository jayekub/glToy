out int vertexId;

void main()  {
    vertexId = gl_VertexID;

    gl_Position = ftransform();
}
