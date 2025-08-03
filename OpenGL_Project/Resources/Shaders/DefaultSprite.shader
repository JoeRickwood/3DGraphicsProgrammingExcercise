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
		FragTexCoords = TexCoords;
		FragNormal = mat3(transpose(inverse(ModelMatrix))) * Normal;
		FragPos = vec3(ModelMatrix * vec4(Position, 1.0f));

        gl_Position = VP * ModelMatrix * vec4(Position, 1.0f);
	}

#elif defined(COMPILING_FS)


    //IN / OUTS
    in vec2 FragTexCoords;
    in vec3 FragNormal;
    in vec3 FragPos;

    out vec4 FinalColor;

    //BASIC
    uniform sampler2D Texture0;
    uniform vec2 Tiling;

    uniform vec3 Ambient;

    void main() 
    {
        vec4 mainCol = texture(Texture0, FragTexCoords * Tiling);

        FinalColor = mainCol * vec4(Ambient, 1.0f);
    }

#endif