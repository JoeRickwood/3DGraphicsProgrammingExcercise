#ifdef COMPILING_VS

	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec2 TexCoords;
	layout (location = 2) in vec3 Normal;
	layout (location = 3) in mat4 ModelMatrix;

	uniform mat4 VP;

	out vec2 FragTexCoords;
	out vec3 FragNormal;
	out vec3 FragPos;


	void main() 
	{
		gl_Position = VP * ModelMatrix * vec4(Position, 1.0f);

		FragTexCoords = TexCoords;
		FragPos = vec3(ModelMatrix * vec4(Position, 1.0f));
	}

#elif defined(COMPILING_FS)

	in vec2 FragTexCoords;
	in vec3 FragNormal;
	in vec3 FragPos;

	uniform sampler2D Texture0;
	uniform vec2 Tiling;
	uniform vec4 Color          = vec4(1.0f, 1.0f, 1.0f, 1.0f);


	out vec4 FinalColor;


	void main() 
	{
		//vec4 mainCol = texture(Texture0, FragTexCoords * Tiling);
		//mainCol.a = 1;
		vec4 mainCol = Color;

		//if(mainCol.a < 0.5)
			//discard;

		FinalColor = mainCol;
	}

#endif