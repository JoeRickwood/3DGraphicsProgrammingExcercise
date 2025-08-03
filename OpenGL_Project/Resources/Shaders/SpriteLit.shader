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
    uniform sampler2D ShadowMap;
    uniform vec2 ScreenSize;

    uniform vec2 Tiling;

    uniform vec3 Ambient;

    float CalculateShadowValue(vec2 coord) 
    {
        return texture(ShadowMap, coord).r;
    }

    float Raymarch(vec2 coord) 
    {
        int RaymarchCount = 25;
        float RayMarchStep = 0.001f; 
        vec2 lightingDir = vec2(0.5, -1);

        float total = 0;

        for(int i = 0; i < RaymarchCount; i++) 
        {
            if(CalculateShadowValue(coord + (-lightingDir * RayMarchStep * i)) > 0.0f) 
            {
                total += 0.1f;
            }
        }

        return 1 - total;
    }

    void main() 
    {
        float shadowValue = Raymarch(gl_FragCoord.xy / ScreenSize);

        vec4 mainCol = texture(Texture0, FragTexCoords * Tiling) * vec4(shadowValue, shadowValue, shadowValue, 1.0f);
        FinalColor = mainCol;
        //FinalColor = vec4(gl_FragCoord.x / ScreenSize.x, gl_FragCoord.y / ScreenSize.y, 0, 1);
    }

#endif