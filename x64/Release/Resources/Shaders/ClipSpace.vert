#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPos;


void main() 
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0f);

	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	FragTexCoords = TexCoords;
	FragNormal = mat3(transpose(inverse(ModelMatrix))) * Normal;
	FragPos = vec3(ModelMatrix * vec4(Position, 1.0f));
}