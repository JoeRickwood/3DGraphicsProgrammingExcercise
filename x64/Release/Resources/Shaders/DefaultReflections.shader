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

    in vec2 FragTexCoords;
    in vec3 FragNormal;
    in vec3 FragPos;

    uniform sampler2D Texture0;
    uniform samplerCube SkyboxTex;
    uniform sampler2D ReflectionTex;

    uniform vec2 Tiling;

    uniform float AmbientStrength            = 0.5f;
    uniform vec3 AmbientColor                = vec3(1.f, 1.f, 1.f);

    uniform vec3 LightColor                  = vec3(1.f, 1.f, 1.f);
    uniform vec3 LightPos                    = vec3(-300.f, 0.f, 100.f);

    uniform vec3 CameraPos;
    uniform float LightSpecularStrength     = 0.5f;
    uniform float ObjectShininess           = 15f;


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

        vec3 ViewDir = normalize(FragPos - CameraPos);
        vec3 ReflectDir = reflect(ViewDir, Normal);

         vec4 mainCol = Light * texture(Texture0, FragTexCoords * Tiling);

        if(mainCol.a < 0.5)
            discard;
   
        vec4 reflectionCol = texture(SkyboxTex, ReflectDir);

        vec4 refl = texture(ReflectionTex, FragTexCoords * Tiling);

        FinalColor = mix(mainCol, reflectionCol, refl.a > 0.1f ? refl.r : 0f);
    }

#endif