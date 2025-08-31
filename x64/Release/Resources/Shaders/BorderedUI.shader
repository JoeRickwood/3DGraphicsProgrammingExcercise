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

	uniform vec2 TextureSize = vec2(64, 64);
	uniform float BorderSize = 50.0f;
	uniform vec2 WidgetSize = vec2(150, 75);	

	out vec4 FinalColor; 

	void main() 
	{
		vec2 texUV = FragTexCoords;
		vec2 pxUV = FragTexCoords * WidgetSize;

		// Compute edge boundaries in widget space
		float left = BorderSize;
		float right = WidgetSize.x - BorderSize;
		float top = BorderSize;
		float bottom = WidgetSize.y - BorderSize;

		// Compute edge boundaries in texture UV space
		vec2 texLeft = vec2(BorderSize, BorderSize) / TextureSize;
		vec2 texRight = vec2(TextureSize.x - BorderSize, TextureSize.y - BorderSize) / TextureSize;

		// Horizontal
		if (pxUV.x < left) 
		{
			texUV.x = pxUV.x / left * texLeft.x;
		} else if (pxUV.x > right) 
		{
			texUV.x = texRight.x + (pxUV.x - right) / BorderSize * (1.0 - texRight.x);
		} else 
		{
			float stretchX = (pxUV.x - left) / (right - left);
			texUV.x = mix(texLeft.x, texRight.x, stretchX);
		}

		// Vertical
		if (pxUV.y < top) 
		{
			texUV.y = pxUV.y / top * texLeft.y;
		} else if (pxUV.y > bottom) 
		{
			texUV.y = texRight.y + (pxUV.y - bottom) / BorderSize * (1.0 - texRight.y);
		} else 
		{
			float stretchY = (pxUV.y - top) / (bottom - top);
			texUV.y = mix(texLeft.y, texRight.y, stretchY);
		}

		vec4 mainCol = texture(Texture0, texUV) * Color;

		if(mainCol.a < 0.5)
			discard;

		FinalColor = mainCol;
	}

#endif