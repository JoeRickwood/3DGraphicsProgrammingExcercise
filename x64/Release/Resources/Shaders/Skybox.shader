#ifdef COMPILING_VS

	layout (location = 0) in vec3 Position;

	uniform mat4 VP;

	out vec3 FragTexCoords;

	void main() 
	{
		vec4 pos = VP * vec4(Position, 1.0f);
		gl_Position = pos.xyww;
		FragTexCoords = vec3(Position.xyz);
	}

#elif defined(COMPILING_FS)

	uniform samplerCube Texture_Skybox;

	in vec3 FragTexCoords;

	out vec4 FinalColor;


	void main() 
	{
		FinalColor = texture(Texture_Skybox, FragTexCoords);
	}

#endif