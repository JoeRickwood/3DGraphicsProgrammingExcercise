#version 460 core

in vec3 FragColor;
uniform float CurrentTime;

out vec4 FinalColor;

void main() 
{
	FinalColor = mix(vec4(FragColor, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), (sin(CurrentTime) + 1.0f) / 2.0f);
}