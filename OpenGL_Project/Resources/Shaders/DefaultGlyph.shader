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
	}

#elif defined(COMPILING_FS)

	in vec2 FragTexCoords;
	in vec3 FragNormal;
	in vec3 FragPos;

	uniform sampler2D Texture0;
	uniform vec4 Color				= vec4(1.0f, 1.0f, 1.0f, 1.0f);


	out vec4 FinalColor;


	void main() 
	{
		float a = texture(Texture0, FragTexCoords).r;

		if(a < 0.5)
			discard;

		vec4 mainCol = Color;

		FinalColor = mainCol;
	}

#endif