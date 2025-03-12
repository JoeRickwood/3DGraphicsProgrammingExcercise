#version 460 core

in vec3 FragColor;
in vec2 FragTexCoords;

uniform sampler2D Texture0;

uniform vec2 UVFrameBottomLeft;
uniform vec2 UVFrameTopRight;

out vec4 FinalColor;

void main() 
{
	
	    // Map fragment UVs into the sprite's UV range

        float u = mix(UVFrameBottomLeft.x, UVFrameTopRight.x, FragTexCoords.x);
        float v = mix(UVFrameBottomLeft.y, UVFrameTopRight.y, FragTexCoords.y);

    	// Sample texture from the correct region
    	FinalColor = texture(Texture0, vec2(u, v));
}