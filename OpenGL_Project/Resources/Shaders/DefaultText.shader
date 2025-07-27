#ifdef COMPILING_VS

	layout (location = 8) in vec4 vertex;

	uniform mat4 VP;

	out vec2 FragTexCoords;


	void main() 
	{
		gl_Position = VP * vec4(vertex.xy, 0.0f, 1.0f);

		FragTexCoords = vertex.zw;
	}

#elif defined(COMPILING_FS)

	in vec2 FragTexCoords;

	uniform sampler2D Texture0;
	uniform vec4 Color				= vec4(1.0f, 1.0f, 1.0f, 1.0f);

	out vec4 FinalColor;

	void main() 
	{
		if(texture(Texture0, FragTexCoords).r < 0.5f) 
		{
			discard;
		}

		FinalColor = Color;
	}

#endif