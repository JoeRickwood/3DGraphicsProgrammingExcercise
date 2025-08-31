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
	uniform sampler2D Noise;
    uniform sampler2D Texture0;
	uniform float Time;

    void main() 
    {
        vec4 mainCol = texture(Texture0, FragTexCoords);

        vec2 uv = FragTexCoords.xy;

	    vec3 raintex = (texture(Noise, vec2(uv.x * 2.0f, uv.y * 0.1f + Time * 0.125f)).rgb / 32.0f);

	    vec2 where = (uv.xy - raintex.xy);

	    vec3 texchur1 = texture(Texture0, vec2(where.x, where.y)).rgb;
	

		//FinalColor = vec4(uv.x, uv.y, 1.0f, 1.0f);
	    FinalColor = vec4(texchur1, 1.0f);
    }

#endif