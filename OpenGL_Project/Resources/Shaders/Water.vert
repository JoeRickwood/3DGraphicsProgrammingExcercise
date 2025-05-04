#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform float WaterTime;
uniform float WaterFrequency = 10f;
uniform float WaterScale = 10f;

out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPos;

float getHeight(float x) 
{
    return sin((WaterTime + x) * WaterFrequency) * WaterScale;
}


void main() 
{
	vec3 addPos = vec3(0f, sin((WaterTime + Position.x ) * WaterFrequency) * WaterScale, 0f);

	vec3 worldPos = Position + addPos;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(worldPos, 1.0f);

	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	FragTexCoords = TexCoords;

	FragNormal = mat3(transpose(inverse(ModelMatrix))) * Normal;

	FragPos = vec3(ModelMatrix * vec4(Position, 1.0f));
}