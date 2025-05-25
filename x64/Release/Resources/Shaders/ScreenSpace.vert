#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;

uniform mat4 ModelMatrix;

out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPos;


void main() 
{
	gl_Position = ModelMatrix * vec4(Position, 1.0f);

	FragTexCoords = TexCoords;
	FragPos = vec3(ModelMatrix * vec4(Position, 1.0f));
}