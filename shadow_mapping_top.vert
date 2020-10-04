#version 420 core
layout (location = 0) in vec3 attrib_position;
//layout (location = 1) in vec3 attrib_normal;
//layout (location = 2) in vec2 texCoords;
in int gl_VertexID;

out vec2 TexCoords;

out  VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 proj_Matrix;
uniform mat4 mv_Matrix;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = proj_Matrix*mv_Matrix * vec4(attrib_position, 1.0f);
    vs_out.FragPos = vec3(attrib_position);
    //vs_out.Normal = transpose(inverse(mat3(model))) * normal;  //法线变换
    vs_out.Normal=vec3(0.0,0.0,1.0);
    //vs_out.TexCoords = texCoords;
    vs_out.TexCoords=attrib_position.xy/20;

    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

}
