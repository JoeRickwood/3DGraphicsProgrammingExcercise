#pragma once
#include "Components.h"

class GeometryBuffer
{
public:
	GeometryBuffer();
	~GeometryBuffer();

	void Bind();
	void Unbind();

	void UpdateTextures();
	void PopulateProgram();

	void WriteDepth();

protected:
	GLuint FBO;

	GLuint Texture_Position;
	GLuint Texture_Normal;
	GLuint Texture_AlbedoShininess;
	GLuint Texture_Depth;
};

