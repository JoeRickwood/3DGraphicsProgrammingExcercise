#ifdef COMPILING_VS
	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec2 TexCoords;
	layout (location = 2) in vec3 Normal;
    layout (location = 3) in mat4 ModelMatrix;

	uniform mat4 lightVP;

	void main() 
	{
		gl_Position = lightVP * ModelMatrix * vec4(Position, 1.0f);
	}

#elif defined(COMPILING_FS)

    out vec4 FinalColor;

	float linearize_depth(float d,float zNear,float zFar)
	{
		float z_n = 2.0 * d - 1.0;
		return (2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear))) / zFar;
	}

    void main() 
    {
		float depth = linearize_depth(gl_FragCoord.z, 0.1f, 100f);

        FinalColor = vec4(vec3(depth), 1.0f);
    }

#endif