#ifdef COMPILING_VS
	layout (location = 0) in vec3 Position;
    layout (location = 3) in mat4 ModelMatrix;

	uniform mat4 LightVP;

	void main() 
	{
		gl_Position = LightVP * ModelMatrix * vec4(Position, 1.0f);
	}

#elif defined(COMPILING_FS)

	out vec4 FinalColor;

    void main() 
    {
		gl_FragDepth = gl_FragCoord.z;

		FinalColor = vec4(vec3(gl_FragCoord.z), 1.0f); 
    }

#endif