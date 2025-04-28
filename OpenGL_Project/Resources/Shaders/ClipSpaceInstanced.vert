#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;

layout (location = 2) in vec4 InstancedMVP0;
layout (location = 3) in vec4 InstancedMVP1;
layout (location = 4) in vec4 InstancedMVP2;
layout (location = 5) in vec4 InstancedMVP3;

out vec2 FragTexCoords;

void main() 
{
	mat4 InstancedMVP = mat4(
		InstancedMVP0,
		InstancedMVP1,
		InstancedMVP2,
		InstancedMVP3
	);

	gl_Position = InstancedMVP * vec4(Position, 1.0f);

	FragTexCoords = TexCoords;
}