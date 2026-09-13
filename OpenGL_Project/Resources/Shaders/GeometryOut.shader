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

	void main() 
	{
		FragTexCoords = TexCoords;
		FragNormal = mat3(transpose(inverse(ModelMatrix))) * Normal;
		FragPos = vec3(ModelMatrix * vec4(Position, 1.0f));

        gl_Position = VP * ModelMatrix * vec4(Position, 1.0f);
	}

#elif defined(COMPILING_FS)

	layout(location = 0) out vec4 Texture_Position;
	layout(location = 1) out vec4 Texture_Normal;
	layout(location = 2) out vec4 Texture_AlbedoShininess;

	in vec2 FragTexCoords;
	in vec3 FragNormal;
	in vec3 FragPos;

	uniform sampler2D Texture0;
	uniform vec2 Tiling = vec2(1, 1);
	uniform vec4 Color          = vec4(1.0f, 1.0f, 1.0f, 1.0f);	
	uniform float ObjectShininess = 1.0;

	out vec4 FinalColor; 

	void main() 
	{
		Texture_Position = vec4(FragPos, 1.0f);
		Texture_Normal = vec4(normalize(FragNormal), 1.0f);
		Texture_AlbedoShininess.rgb = texture(Texture0, FragTexCoords * Tiling).rgb;
		Texture_AlbedoShininess.a = ObjectShininess;

		vec4 mainCol = texture(Texture0, FragTexCoords) * Color;

		if(mainCol.a < 0.5)
			discard;

		FinalColor = mainCol;
	}

#endif