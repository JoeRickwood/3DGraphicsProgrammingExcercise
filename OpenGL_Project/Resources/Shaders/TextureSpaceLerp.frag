#version 460 core

in vec3 FragColor;
in vec2 FragTexCoords;

uniform sampler2D Texture0;
uniform sampler2D Texture1;

uniform float Time;

uniform vec2 UVFrameBottomLeft;
uniform vec2 UVFrameTopRight;

out vec4 FinalColor;

void main() 
{
	// Map fragment UVs into the sprite's UV range

        float u = mix(UVFrameBottomLeft.x, UVFrameTopRight.x, FragTexCoords.x);
        float v = mix(UVFrameBottomLeft.y, UVFrameTopRight.y, FragTexCoords.y);

    	// Sample texture from the correct region

        vec4 color = mix(texture(Texture0, vec2(u, v)), texture(Texture1, vec2(u, v)), (sin(Time) + 1f) * 0.5f);

    	FinalColor = mix(texture(Texture0, vec2(u, v)), texture(Texture1, vec2(u, v)), (sin(Time) + 1f) * 0.5f);
}