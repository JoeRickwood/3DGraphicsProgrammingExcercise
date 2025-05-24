#version 460 core

in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

uniform sampler2D Texture0;
uniform vec2 Tiling;

uniform float AmbientStrength            = 0.5f;
uniform vec3 AmbientColor                = vec3(1.f, 1.f, 1.f);

uniform vec3 LightColor                  = vec3(0.5f, 0.5f, 0.5f);
uniform vec3 LightPos                    = vec3(-300.f, 0.f, 100.f);

uniform vec3 CameraPos;
uniform float LightSpecularStrength     = 0.2f;
uniform float ObjectShininess           = 10.0f;


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


    if(texture(Texture0, FragTexCoords * Tiling).a < 0.5)
        discard;

    vec4 col = texture(Texture0, FragTexCoords * Tiling) * Light;

    float val = FragTexCoords.y * 2f;

    col.a = clamp(val * val * val, 0f, 1f);

    FinalColor = col;
}