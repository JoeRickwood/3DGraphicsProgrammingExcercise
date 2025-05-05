#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;
layout (location = 3) in mat4 Model;

uniform mat4 VP;

out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPos;

void main() 
{
	gl_Position = (VP * Model) * vec4(Position, 1.0f);

	FragTexCoords = TexCoords;
	FragNormal = mat3(transpose(inverse(Model))) * Normal;
	FragPos = vec3(Model * vec4(Position, 1.0f));
}