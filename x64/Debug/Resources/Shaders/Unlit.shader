#ifdef COMPILING_VS
	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec2 TexCoords;
	layout (location = 2) in vec3 Normal;
    layout (location = 3) in mat4 ModelMatrix;

	uniform mat4 VP;

	void main() 
	{
        gl_Position = VP * ModelMatrix * vec4(Position, 1.0f);
	}

#elif defined(COMPILING_FS)

    out vec4 FinalColor;

    void main() 
    {
        FinalColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    }

#endif