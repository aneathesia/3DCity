#version 420 core
in vec4 Color;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform vec3 HalfVector;
uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
    float ambientStrength=0.5;
    float Shininess=20.0f;
    float Strength=0.5f;

    vec3 ambient = ambientStrength*lightColor;

    float diffuse= max(0.0f,dot(Normal,lightDirection));
    float specular=max(0.0f,dot(Normal,HalfVector));

    if(diffuse==0)
        specular=0.0;
    else
        specular=pow(specular,Shininess);

    vec3 scatteredlight=ambient+lightColor*diffuse;
    vec3 reflectedlight=lightColor*specular*Strength;

    vec3 rgb=min(Color.rgb*scatteredlight+reflectedlight,vec3(1.0));

    vec4 tex=texture2D(ourTexture,TexCoord)*Color;

    //gl_FragColor=vec4(Color.a);
    FragColor=vec4(rgb*tex.rgb,Color.a);
}
