#version 420 core
in vec2 TexCoord;
uniform sampler2D shadow2D;
void main()
{
    //gl_FragDepth = gl_FragCoord.z;
    vec4 tex=texture2D(shadow2D,TexCoord);
    gl_FragColor= vec4(tex.rgb,1.0);
}
