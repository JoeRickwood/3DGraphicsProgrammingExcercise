#pragma once
#include "Renderer.h"

class Terrain : public Renderer
{
protected:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<int> indices;

	int terrainID;
	glm::ivec2 terrainSize;
	float cellSpacing = 1.0f;

	float terrainScale;

	std::vector<float> heights;

	Mesh* mesh;

	glm::mat4 modelMat;

public:
	Terrain(std::string _shaderKey, ProjectionType _projection, int _sizeX, int _sizeY, float _cellSpacing);
	~Terrain();

	void Init()override;
	void InitVBO()override;
	void BindVBOData()override;

	void GenerateMesh(float _scale);

	float SampleHeight(float _x, float _y);
	float SampleSteepness(float _x, float _y);

	float GetCellSpacing();
	glm::vec2 GetSize();

	float Smoothstep(float edge0, float edge1, float x);

	void Update()override;
	void Render()override;
};