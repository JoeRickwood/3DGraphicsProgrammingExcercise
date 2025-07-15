#ifdef COMPILING_VS
	layout (location = 0) in vec3 Position;
    layout (location = 3) in mat4 ModelMatrix;

	uniform mat4 VP;

	void main() 
	{
		gl_Position = VP * ModelMatrix * vec4(Position, 1.0f);
	}

#elif defined(COMPILING_FS)

	out vec4 FinalColor;

	float linearize_depth(float d,float zNear,float zFar)
    {
        float z_n = 2.0 * d - 1.0;
        return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
    }

    void main() 
    {
		gl_FragDepth = gl_FragCoord.z;

		float depth = linearize_depth(gl_FragCoord.z, 0.1f, 100.f) / 100.f;
		depth = floor(depth * 10.f) / 10.f;

		FinalColor = vec4(vec3(depth), 1.0f); 
    }

#endif