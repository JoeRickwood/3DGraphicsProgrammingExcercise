#ifdef COMPILING_VS
	layout (location = 0) in vec4 Position;

	uniform mat4 VP;

	out float LifeTime;

	void main() 
	{
        gl_Position = VP * vec4(Position.xyz, 1.0f);

		gl_PointSize = 3.0;

		uint Index = gl_VertexID;

		LifeTime = Position.w;
	}

#elif defined(COMPILING_FS)

    out vec4 FinalColor;

	uniform vec4 Color;

	in float LifeTime;
	uniform float FadeSpeed = 5.0f;

    void main() 
    {
        FinalColor = vec4(Color.xyz, LifeTime);
    }

#endif