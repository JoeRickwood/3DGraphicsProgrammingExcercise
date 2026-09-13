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
    out vec4 FragPosLightSpace;

	void main() 
	{
		FragTexCoords = TexCoords;
		FragNormal = mat3(transpose(inverse(ModelMatrix))) * Normal;
		FragPos = vec3(ModelMatrix * vec4(Position, 1.0f));
        FragPosLightSpace = LightVP * vec4(FragPos, 1.0);

        gl_Position = VP * ModelMatrix * vec4(Position, 1.0f);
	}

#elif defined(COMPILING_FS)

    //IN / OUTS
    in vec2 FragTexCoords;
    in vec3 FragNormal;
    in vec3 FragPos;
    in vec4 FragPosLightSpace;

    out vec4 FinalColor;

    //BASIC
    uniform sampler2D Texture0;

    void main() 
    {
        vec4 mainCol = texture(Texture0, FragTexCoords);

        float greyScale = mainCol.r * 0.3f + mainCol.g * 0.11f + mainCol.b * 0.59f;
        vec4 color = vec4(vec3(greyScale), mainCol.a);

        if(mainCol.a < 0.5)
            discard;

        FinalColor = color;
    }

#endif