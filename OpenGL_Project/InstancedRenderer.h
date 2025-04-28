#pragma once
#include "Renderer.h"

class InstancedRenderer : public Renderer
{
private:	
	int drawCount;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> rotations;
	std::vector<glm::vec3> scales;

	std::vector<glm::mat4> InstancedMVPs;

	GLuint VBO_Instanced;

public:
	InstancedRenderer(int _type = 0, ShaderType _shader = ShaderType::Texture, int _textureID = 0, ProjectionType _projectionType = ProjectionType::Perspective);
	~InstancedRenderer();

	void AddInstance(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

	void InitInstancing();
	void Render()override;
};

