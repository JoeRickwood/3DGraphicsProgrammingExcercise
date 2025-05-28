#ifdef COMPILING_VS
	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec2 TexCoords;
	layout (location = 2) in vec3 Normal;

	uniform mat4 ModelMatrix;
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;

	out vec2 FragTexCoords;
	out vec3 FragNormal;
	out vec3 FragPos;


	void main() 
	{
		gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0f);

		FragTexCoords = TexCoords;
		FragNormal = mat3(transpose(inverse(ModelMatrix))) * Normal;
		FragPos = vec3(ModelMatrix * vec4(Position, 1.0f));
	}

#elif defined(COMPILING_FS)
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
        float Range;
    };


    //IN / OUTS
    in vec2 FragTexCoords;
    in vec3 FragNormal;
    in vec3 FragPos;

    out vec4 FinalColor;

    //BASIC
    uniform sampler2D Texture0;
    uniform vec2 Tiling;

    uniform vec3 Ambient;

    uniform vec3 CameraPos;
    uniform float Smoothness                 = 5f;


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
        vec3 normal = normalize(FragNormal);
        vec3 lightDir = normalize(-DirLight.Direction); // assumes DirLight.Direction points *from* light
        vec3 viewDir = normalize(CameraPos - FragPos);

        // Diffuse
        float diffuseStrength = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diffuseStrength * DirLight.Color;

        // Specular (Blinn-Phong)
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), Smoothness);
        vec3 specular = spec * DirLight.SpecularStrength * DirLight.Color;

        return diffuse + specular;
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

        intensity *= min(1.0f, SpotLights[index].Range / length(SpotLights[index].Position - FragPos));

        return SpotLights[index].Color * intensity;
    }

    void main() 
    {
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
        TotalLight += Ambient;

        vec4 mainCol = vec4(TotalLight, 1.0f) * texture(Texture0, FragTexCoords * Tiling);

        if(mainCol.a < 0.5)
            discard;

        FinalColor = mainCol;
    }

#endif