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

		FragTexCoords = TexCoords;
		FragPos = vec3(ModelMatrix * vec4(Position, 1.0f));
	}

#elif defined(COMPILING_FS)

	in vec2 FragTexCoords;
	in vec3 FragNormal;
	in vec3 FragPos;

	uniform sampler2D Texture0;
	uniform vec4 Color          = vec4(1.0f, 1.0f, 1.0f, 1.0f);	

	uniform float Time;

	out vec4 FinalColor; 

	float smoothstep(float edge0, float edge1, float x) 
    {
        // Scale, and clamp x to 0..1 range
        x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
        return x * x * (3.0 - 2.0 * x);
    }

	void main() 
	{

		float val = texture(Texture0, FragTexCoords).r;

		float sinTime = mod(-Time / 2.0f, 1.0f);

		float mult1 = smoothstep(sinTime - 0.1f, sinTime, val);
		float mult2 = 1 - smoothstep(sinTime - 0.05f, sinTime + 0.10f, val);


		FinalColor = Color * mult1 * mult2;
	}

#endif