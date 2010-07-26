mat4x4 translation_matrix(vec3 t)
{
    return mat4x4(1., 0., 0., t.x,
                  0., 1., 0., t.y,
                  0., 0., 1., t.z,
                  0., 0., 0., 1.);
}

mat4x4 rotation_matrix(float angle, vec3 axis)
{
    float s = sin(angle);
    float c = cos(angle);
    float omc = 1. - c;

    vec3 An = normalize(axis);
    float x = An.x;
    float y = An.y;
    float z = An.z;

    return mat4x4(
        x * x * omc + c,
        y * x * omc - z * s,
        z * x * omc + y * s,
        0.,

        x * y * omc + z * s,
        y * y * omc + c,
        z * y * omc - x * s,
        0.,

        x * z * omc - y * s,
        y * z * omc + x * s,
        z * z * omc + c,
        0.,

        0., 0., 0., 1.);
}

mat4x4 scale_matrix(vec3 s)
{
    return mat4x4(s.x, 0., 0., 0.,
                  0., s.y, 0., 0.,
                  0., 0., s.z, 0.,
                  0., 0., 0., 1.);
}
