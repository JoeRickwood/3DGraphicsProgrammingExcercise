#version 460 core

in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;


uniform sampler2D Texture0;

uniform float AmbientStrength            = 0.15f;
uniform vec3 AmbientColor                = vec3(1.f, 1.f, 1.f);

uniform vec3 LightColor                  = vec3(1.f, 1.f, 1.f);
uniform vec3 LightPos                    = vec3(-300.f, 0.f, 100.f);

uniform vec3 CameraPos;
uniform float LightSpecularStrength     = 1.0f;
uniform float ObjectShininess           = 100.0f;


out vec4 FinalColor;

void main() 
{
    vec3 Ambient = AmbientStrength * AmbientColor;

    vec3 Normal = normalize(FragNormal);
    vec3 LightDir = normalize(FragPos - LightPos);

    float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);
    vec3 Diffuse = DiffuseStrength * LightColor;

    vec3 ReverseViewDir = normalize(CameraPos - FragPos);
    vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
    float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), ObjectShininess);
    vec3 Specular = LightSpecularStrength * SpecularReflectivity * LightColor;

    vec4 Light = vec4(Ambient + Diffuse + Specular, 1.0f);

    FinalColor = Light * texture(Texture0, FragTexCoords);
    //FinalColor = vec4(FragNormal, 1.0f);
}