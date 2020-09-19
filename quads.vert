#version 420 core
layout (location = 0) in vec3 attrib_position;
uniform mat4 proj_Matrix;
uniform mat4 mv_Matrix;


void main()
{
    gl_Position = proj_Matrix*mv_Matrix* vec4(attrib_position, 1.0f);
}
