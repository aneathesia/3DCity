#version 420 core
in vec4 Color;
in vec3 Normal;
in vec4 mPosition;
in vec2 TexCoord;

uniform vec3 lightColor;
uniform vec3 lightPosition;
//uniform vec3 HalfVector;
uniform sampler2D ourTexture;

uniform vec3 EyeDirection;


out vec4 FragColor;

void main()
{
    float ambientStrength=0.5;
    float Shininess=8.0f;
    float Strength=0.5f;
    float ConstantAttenuation=0.5;
    float LinearAttenuation=0.5;
    float QuadraticAttenuation=0.5;
    vec3 ambient = ambientStrength*lightColor;

    vec3 lightDirection=lightPosition-mPosition.xyz;
    float lightDistance=length(lightDirection);
    lightDirection=lightDirection/lightDistance;
    float attenuation=1.0/(ConstantAttenuation+
                           LinearAttenuation*lightDistance+
                           QuadraticAttenuation*lightDistance*lightDistance);
    vec3 HalfVector=normalize(lightDirection+EyeDirection);
    float diffuse= max(0.0f,dot(Normal,lightDirection));
    float specular=max(0.0f,dot(Normal,HalfVector));

    if(diffuse==0)
        specular=0.0;
    else
        specular=pow(specular,Shininess)*Strength;

    vec3 scatteredlight=ambient+lightColor*diffuse*attenuation;
    vec3 reflectedlight=lightColor*specular*attenuation;

    vec3 rgb=min(Color.rgb*scatteredlight+reflectedlight,vec3(1.0));

    vec4 tex=texture2D(ourTexture,TexCoord)*Color;

    //gl_FragColor=vec4(Color.a);
    FragColor=vec4(rgb,Color.a);


}
