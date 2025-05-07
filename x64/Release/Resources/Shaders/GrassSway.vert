#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;
layout (location = 3) in mat4 Model;

uniform mat4 VP;
uniform float Time;

out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPos;


float power(float inVal, int inPow) 
{
	for (int i = 0; i < inPow; i++) 
	{
		inVal *= inVal;
	}

	return inVal;
}

void main() 
{
	vec4 worldPos = Model * vec4(Position, 1.0);

	float swayX = (((sin(Time * 2f) + 0.5f) / 2.f) * TexCoords.y * 0.3) * (sin(worldPos.x + Time) + 1f);
	float swayY = ((cos(Time * 2f) + 0.5f) / 2.f) * TexCoords.y * 0.1f;

	worldPos.x += swayX;
	worldPos.z += swayY;

	gl_Position = VP * worldPos;

	FragTexCoords = TexCoords;
	FragNormal = mat3(transpose(inverse(Model))) * Normal;
	FragPos = vec3(worldPos);
}