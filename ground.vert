#version 420 core
layout (location = 0) in vec3 attrib_position;

in int gl_VertexID;
out vec2 TexCoord;
uniform mat4 proj_Matrix;
uniform mat4 mv_Matrix;


void main()
{
    const vec2 uvData[4]=vec2[4](
        vec2(0.0,1.0), vec2(1.0,1.0), vec2(1.0,0.0),vec2(0.0,0.0)   //0 1 3 2
                             );
    TexCoord=uvData[gl_VertexID%4];
    gl_Position = proj_Matrix*mv_Matrix* vec4(attrib_position, 1.0f);
}
