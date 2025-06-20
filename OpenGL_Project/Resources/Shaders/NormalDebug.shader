#ifdef COMPILING_VS
	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec2 TexCoords;
	layout (location = 2) in vec3 Normal;
    layout (location = 3) in mat4 ModelMatrix;

	uniform mat4 VP;

	out vec3 FragNormal;

	void main() 
	{
		FragNormal = mat3(transpose(inverse(ModelMatrix))) * Normal;

        gl_Position = VP * ModelMatrix * vec4(Position, 1.0f);
	}

#elif defined(COMPILING_FS)

    //IN / OUTS
    in vec3 FragNormal;


    out vec4 FinalColor;

    void main() 
    {
		vec3 normal = normalize(FragNormal); // ensure it's normalized
		vec3 normalColor = normal * 0.5 + 0.5;

        FinalColor = vec4(normalColor, 1f);
    }

#endif