#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;

layout (location = 3) in vec4 Model0;
layout (location = 4) in vec4 Model1;
layout (location = 5) in vec4 Model2;
layout (location = 6) in vec4 Model3;

uniform mat4 VP;

out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPos;

void main() 
{
	mat4 Model = mat4(
		Model0,
		Model1,
		Model2,
		Model3
	);

	gl_Position = (VP * Model) * vec4(Position, 1.0f);

	FragTexCoords = TexCoords;
	FragNormal = mat3(transpose(inverse(Model))) * Normal;
	FragPos = vec3(Model * vec4(Position, 1.0f));
}