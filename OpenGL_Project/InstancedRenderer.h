#pragma once
#include "Renderer.h"

class InstancedRenderer : public Renderer
{
private:	
	int drawCount;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> rotations;
	std::vector<glm::vec3> scales;

	std::vector<glm::mat4> ModelMatrixes;


public:
	InstancedRenderer(std::string _shaderKey, ProjectionType _projectionType);
	~InstancedRenderer();

	void Init()override;
	void InitVBO()override;
	void AddInstance(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

	void Render() override;
	void Render(std::string _shaderKeyOverride) override;
};

