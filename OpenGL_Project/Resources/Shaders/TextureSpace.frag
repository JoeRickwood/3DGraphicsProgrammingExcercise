#version 460 core

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4


//STRUCTURES USED
struct PointLight 
{
    vec3 Position;
    vec3 Color;
    float SpecularStrength;

    float AttenuationConstant;
    float AttenuationLinear;
    float AttenuationExponent;
};

struct DirectionalLight 
{
	vec3 Direction;
	vec3 Color;
	float SpecularStrength;
};

struct SpotLight 
{
	vec3 Position;
	vec3 Direction;
    vec3 Color;

	float InnerCone;
	float OuterCone;
};


//IN / OUTS
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

out vec4 FinalColor;

//BASIC
uniform sampler2D Texture0;
uniform vec2 Tiling;

uniform float AmbientStrength            = 0.5f;
uniform vec3 AmbientColor                = vec3(1.f, 1.f, 1.f);

uniform vec3 CameraPos;
uniform float Smoothness                 = 15f;


//LIGHTS
uniform unsigned int PointLightCount;
uniform PointLight PointLights[MAX_POINT_LIGHTS];

uniform DirectionalLight DirLight;

uniform unsigned int SpotLightCount;
uniform SpotLight SpotLights[MAX_SPOT_LIGHTS];

vec3 CalculateLightPoint(unsigned int index) 
{
    vec3 Normal = normalize(FragNormal);
    vec3 LightDir = normalize(FragPos - PointLights[index].Position);

    float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);
    vec3 Diffuse = DiffuseStrength * PointLights[index].Color;

    vec3 ReverseViewDir = normalize(CameraPos - FragPos);
    vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
    float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), Smoothness);
    vec3 Specular = PointLights[index].SpecularStrength * SpecularReflectivity * PointLights[index].Color;


    float Distance = length(PointLights[index].Position - FragPos);
    float Attenuation = PointLights[index].AttenuationConstant + (PointLights[index].AttenuationLinear * Distance) + (PointLights[index].AttenuationExponent * pow(Distance, 2));

    return (Diffuse + Specular) / Attenuation;
}

vec3 CalculateLightDirectional() 
{
    vec3 Normal = normalize(FragNormal);

    vec3 LightDir = normalize(DirLight.Direction);

    float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);
    vec3 Diffuse = DiffuseStrength * DirLight.Color;

    vec3 ReverseViewDir = normalize(CameraPos - FragPos);
    vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
    float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), Smoothness);
    vec3 Specular = DirLight.SpecularStrength * SpecularReflectivity * DirLight.Color;

    return (Diffuse + Specular);
}

vec3 CalculateSpotLight(unsigned int index) 
{
    vec3 LightDirNormalized = normalize(SpotLights[index].Position - FragPos); 
    vec3 SpotLightDirNormalized = normalize(SpotLights[index].Direction);

    float theta = dot(LightDirNormalized, SpotLightDirNormalized);

    float inner = cos(SpotLights[index].InnerCone);
    float outer = cos(SpotLights[index].OuterCone);

    float intensity = 0.f;
    if (theta > inner)
    {
        intensity = 1.0;
    }
    else if (theta > outer)
    {
        float epsilon = inner - outer;
        intensity = clamp((theta - outer) / epsilon, 0.0, 1.0);
    }

    return SpotLights[index].Color * intensity;
}

void main() 
{
    vec3 Ambient = AmbientStrength * AmbientColor;


    //Calculate Total Light Amount
    vec3 TotalLight = vec3(0.0f, 0.0f, 0.0f);

    for(unsigned int i = 0; i < PointLightCount; i++) 
    {
        TotalLight += CalculateLightPoint(i);
    }

    for(unsigned int i = 0; i < SpotLightCount; i++)
    {
        TotalLight += CalculateSpotLight(i);
    }

    TotalLight += CalculateLightDirectional();

    vec4 mainCol = vec4(TotalLight, 1.0f) * texture(Texture0, FragTexCoords * Tiling);

    if(mainCol.a < 0.5)
        discard;

    FinalColor = mainCol;
}