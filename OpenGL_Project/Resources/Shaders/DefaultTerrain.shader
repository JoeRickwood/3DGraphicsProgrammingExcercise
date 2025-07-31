#ifdef COMPILING_VS
	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec2 TexCoords;
	layout (location = 2) in vec3 Normal;
    layout (location = 3) in mat4 ModelMatrix;

	uniform mat4 VP;
    uniform mat4 LightVP;

	out vec2 FragTexCoords;
	out vec3 FragNormal;
	out vec3 FragPos;
    out vec4 FragPosLightSpace;

	void main() 
	{
		FragTexCoords = Position.xz;
		FragNormal = mat3(transpose(inverse(ModelMatrix))) * Normal;
		FragPos = vec3(ModelMatrix * vec4(Position, 1.0f));
        FragPosLightSpace = LightVP * vec4(FragPos, 1.0);

        gl_Position = VP * ModelMatrix * vec4(Position, 1.0f);
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
    in vec4 FragPosLightSpace;

    out vec4 FinalColor;

    //BASIC
    uniform sampler2D TextureGrass;
    uniform sampler2D TextureGrassVariant;
    uniform sampler2D TextureGrassVariationNoise;
    uniform sampler2D TextureSand;
    uniform sampler2D TextureRock;
    uniform sampler2D TextureSnow;
    uniform sampler2D ShadowMap;
    uniform sampler2D TextureNoise;

    uniform vec2 Tiling;

    uniform vec3 Ambient;

    uniform vec3 CameraPos;
    uniform float Smoothness                 = 1f;


    //LIGHTS
    uniform unsigned int PointLightCount;
    uniform PointLight PointLights[MAX_POINT_LIGHTS];

    uniform DirectionalLight DirLight;

    uniform unsigned int SpotLightCount;
    uniform SpotLight SpotLights[MAX_SPOT_LIGHTS];

    float ShadowCalculation(vec4 fragPosLightSpace)
    {
         vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

         projCoords = projCoords * 0.5 + 0.5; 

         float currentDepth = projCoords.z;  


         //float bias = max(0.05f * (1.0f - dot(FragNormal, DirLight.Direction)), 0.005f); 
         float bias = 0.0001f;
         float shadow = 0.0f;  

         int sampleRadius = 2;
         vec2 texelSize = 1.0 / vec2(textureSize(ShadowMap, 0));
         for(int x = -sampleRadius; x <= sampleRadius; ++x)
         {
             for(int y = -sampleRadius; y <= sampleRadius; ++y)
             {
                 float value = (projCoords.x > 0 && projCoords.x < 1 && projCoords.x > 0 && projCoords.y < 1) ? 1f : 0f;

                 float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                 shadow += (currentDepth - bias > pcfDepth ? 1.0 : 0.0) * value;        
             }    
         }
         shadow /= pow((sampleRadius * 2) + 1, 2);

         if(projCoords.z > 1.0) 
         {
            shadow = 0.0;
         }

         return shadow * 0.75f;
    } 

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

        //return diffuse + Specular;
        return diffuse;
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

    float linearize_depth(float d,float zNear,float zFar)
    {
        float z_n = 2.0 * d - 1.0;
        return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
    }

    float smoothstep(float edge0, float edge1, float x) 
    {
        // Scale, and clamp x to 0..1 range
        x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
        return x * x * (3.0 - 2.0 * x);
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

        float noise = texture(TextureNoise, FragTexCoords * 0.1f).r * 3.0f;
        float noise2 = texture(TextureGrassVariationNoise, FragTexCoords * 0.001f).r;
        
        vec4 grassCol = mix(texture(TextureGrass, FragTexCoords * Tiling), texture(TextureGrassVariant, FragTexCoords * Tiling), noise2);
        vec4 sandCol = texture(TextureSand, FragTexCoords * Tiling);
        vec4 rockCol = texture(TextureRock, FragTexCoords * Tiling);
        vec4 snowCol = texture(TextureSnow, FragTexCoords * Tiling);

        float angle = dot(FragNormal, vec3(0, 1, 0));
        angle = smoothstep(0.90f, 0.95f, angle);

        float height = smoothstep(1.0f + noise, 1.3f + noise, FragPos.y);
        float heightSnow = smoothstep(30.0f + noise, 30.3f + noise, FragPos.y);

        vec4 combinedColor = mix(rockCol, grassCol, angle);
        combinedColor = mix(sandCol, combinedColor, height);
        combinedColor = mix(combinedColor, snowCol, heightSnow);

        vec4 mainCol = vec4(TotalLight * (1.0 - ShadowCalculation(FragPosLightSpace)), 1.0f) * combinedColor;
        //vec4 mainCol = vec4(TotalLight, 1.0f) * texture(Texture0, FragTexCoords * Tiling);

        if(mainCol.a < 0.5)
            discard;


        //FinalColor = vec4(vec3(floor(col * 10f) / 10f), 1.0f);
        FinalColor = mainCol;
    }

#endif