#ifdef COMPILING_VS

	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec2 TexCoords;
	layout (location = 2) in vec3 Normal;
	layout (location = 3) in mat4 ModelMatrix;

    uniform mat4 LightVP;
	uniform mat4 VP;

	out vec2 FragTexCoords;
    out mat4 FragLightVP;

	void main() 
	{
		gl_Position = VP * ModelMatrix * vec4(Position, 1.0f);
 
        FragLightVP = LightVP;

		FragTexCoords = TexCoords;
	}

#elif defined(COMPILING_FS)

	#define MAX_POINT_LIGHTS 50

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

	//LIGHTS
    uniform unsigned int PointLightCount;
    uniform PointLight PointLights[MAX_POINT_LIGHTS];

    uniform DirectionalLight DirLight;

    uniform sampler2D ShadowMap;

    uniform vec3 CameraPos;

	vec3 CalculateLightPoint(unsigned int index, vec3 FragPos, vec3 FragNormal, float Smoothness) 
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

    vec3 CalculateLightDirectional(vec3 FragNormal, vec3 FragPos, float Smoothness) 
    {
        vec3 normal = normalize(FragNormal);
        vec3 lightDir = normalize(-DirLight.Direction); // assumes DirLight.Direction points *from* light
        vec3 viewDir = normalize(CameraPos - FragPos);

        // Diffuse
        float diffuseStrength = max(dot(normal, lightDir), 0.2f);
        vec3 diffuse = diffuseStrength * DirLight.Color;

        // Specular (Blinn-Phong)
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.2f), Smoothness);
        vec3 specular = spec * DirLight.SpecularStrength * DirLight.Color;

        return diffuse + specular;
    }

    float ShadowCalculation(vec4 fragPosLightSpace)
    {
         vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

         projCoords = projCoords * 0.5 + 0.5; 

         float currentDepth = projCoords.z;  


         //float bias = max(0.05 * (1.0 - dot(FragNormal, DirLight.Direction)), 0.005); 
         float bias = 0.0001f;
         float shadow = 0.0f;  

         int sampleRadius = 2;
         vec2 texelSize = 1.0 / vec2(textureSize(ShadowMap, 0));
         for(int x = -sampleRadius; x <= sampleRadius; ++x)
         {
             for(int y = -sampleRadius; y <= sampleRadius; ++y)
             {
                 float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                 shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
             }    
         }
         shadow /= pow((sampleRadius * 2) + 1, 2);


         if(projCoords.z > 1.0) 
         {
            shadow = 0.0;
         }

         return shadow * 0.75f;
    } 

	in vec2 FragTexCoords;
    in mat4 FragLightVP;

	uniform sampler2D Texture_Position;
	uniform sampler2D Texture_Normal;
	uniform sampler2D Texture_AlbedoShininess;

	out vec4 FinalColor; 

    float linearize_depth(float d,float zNear,float zFar)
    {
        float z_n = 2.0 * d - 1.0;
        return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
    } 


	void main() 
	{
        vec3 FragPos = texture(Texture_Position, FragTexCoords).xyz;
		vec3 FragNormal = texture(Texture_Normal, FragTexCoords).xyz;
		vec3 FragAlbedo = texture(Texture_AlbedoShininess, FragTexCoords).rgb;
		float objSmoothness = texture(Texture_AlbedoShininess, FragTexCoords).a;

		vec3 TotalLight = vec3(0.0f, 0.0f, 0.0f);

        for(unsigned int i = 0; i < PointLightCount; i++) 
        {
            TotalLight += CalculateLightPoint(i, FragPos, FragNormal, objSmoothness);
        }

        TotalLight += CalculateLightDirectional(FragPos, FragNormal, objSmoothness);
		

		FinalColor = vec4(FragAlbedo, 1.0f) * vec4(TotalLight, 1.0f) * (1 - ShadowCalculation(FragLightVP * vec4(FragPos, 1)));

        //float val = linearize_depth(1 - ShadowCalculation(FragLightVP * vec4(FragPos, 1)), 0.1, 500) / 500;

        //FinalColor = vec4(val, val, val, 1);

	}

#endif