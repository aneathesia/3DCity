#version 420 core
layout(location=0) in vec3 attrib_position;
layout(location=1) in vec3 attrib_normal;
in int gl_VertexID;
out vec4 Color;
out vec3 Normal;
out vec2 TexCoord;
out vec4 mPosition;

uniform mat3 NormalMatrix;
uniform mat4 proj_Matrix;
//uniform mat4 view_Matrix;
//uniform mat4 model_Matrix;
uniform mat4 mv_Matrix;
uniform vec4 color;

void main()
{

//    const vec2 uvData[4]=vec2[4](
//     vec2(0.0,1.0), vec2(1.0,1.0), vec2(1.0,0.0),vec2(0.0,0.0)   //0 1 3 2
//                         );
//    TexCoord=uvData[gl_VertexID%4];

    if(gl_VertexID%8==0||gl_VertexID%8==1||gl_VertexID%8==2||gl_VertexID%8==3)
    {
        TexCoord=attrib_position.xz/20;
     }
    else    TexCoord=attrib_position.yz/20;

    Color=color;
    gl_Position=proj_Matrix*mv_Matrix*vec4(attrib_position,1.0);
    //mPosition=vec4(attrib_position,1.0); //mv_Matrix
    //Normal=normalize(NormalMatrix*attrib_normal);
    Normal=attrib_normal;
}

