#version 460 core

in vec3 FragColor;
in vec2 FragTexCoords;

uniform sampler2D Texture0;

out vec4 FinalColor;

void main() 
{
	FinalColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}