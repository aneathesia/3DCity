#version 420 core
layout (location =0 ) in vec3 attrib_position;
uniform mat4 lightSpaceMatrix;
//uniform mat4 model;

void main(){
    //gl_Position= lightSpaceMatrix*model*vec4(attrib_position,1.0);
    gl_Position=lightSpaceMatrix*vec4(attrib_position,1.0);
}
