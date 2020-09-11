#version 420 core
layout(location=0) in vec3 attrib_position;
layout(location=1) in vec2 attrib_texcoord;
out vec4 Color;
out vec3 Normal;
out vec2 TexCoord;

uniform mat3 NormalMatrix;
uniform mat4 proj_Matrix;
//uniform mat4 view_Matrix;
//uniform mat4 model_Matrix;
uniform mat4 mv_Matrix;

void main()
{
    Color=vec4(1.0f,1.0f,1.0f,1.0f);
    TexCoord=attrib_position.xy/20;
    gl_Position=proj_Matrix*mv_Matrix*vec4(attrib_position,1.0);
    Normal=vec3(0,0,1);
    Normal=normalize(NormalMatrix*vec3(0,0,1));

    //Normal=attrib_normal;
}

