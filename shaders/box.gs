// center of cube in
layout(points) in;

// sides of cube out
layout(line_strip, max_vertices = 128) out;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void emit_face(vec4 p0, vec4 p1, vec4 p2, vec4 p3)
{
    gl_Position = p0; EmitVertex();
    gl_Position = p1; EmitVertex();
    gl_Position = p2; EmitVertex();
    gl_Position = p3; EmitVertex();
    gl_Position = p0; EmitVertex();
    EndPrimitive();
}

void emit_side(vec4 p0, vec4 p1)
{
    gl_Position = p0; EmitVertex();
    gl_Position = p1; EmitVertex();
    EndPrimitive();
}

void main()
{
    mat4 mvpMat = projMat * viewMat * modelMat;

    vec4 v[8];
    
    v[0] = mvpMat * vec4(0., 0., 0., 1.);
    v[1] = mvpMat * vec4(1., 0., 0., 1.);
    v[2] = mvpMat * vec4(1., 1., 0., 1.);
    v[3] = mvpMat * vec4(0., 1., 0., 1.);
    
    v[4] = mvpMat * vec4(0., 0., 1., 1.);
    v[5] = mvpMat * vec4(1., 0., 1., 1.);
    v[6] = mvpMat * vec4(1., 1., 1., 1.);
    v[7] = mvpMat * vec4(0., 1., 1., 1.);
    
    emit_face(v[0], v[1], v[2], v[3]); // front
    emit_face(v[4], v[5], v[6], v[7]); // back
    
    emit_side(v[0], v[4]); // bottom left
    emit_side(v[1], v[5]); // bottom right
    emit_side(v[2], v[6]); // top right
    emit_side(v[3], v[7]); // top left
}