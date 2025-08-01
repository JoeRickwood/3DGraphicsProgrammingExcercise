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

	const float minHeight = -5.f;
	const float maxHeight = 50.0f;

	int seed = 297183;

	std::vector<float> heights;

	Mesh* mesh;

	glm::mat4 modelMat;

public:
	Terrain(std::string _shaderKey, ProjectionType _projection, int _sizeX, int _sizeY, float _cellSpacing);
	~Terrain();

	void Init()override;
	void InitVBO()override;
	void BindVBOData()override;

	void GenerateMesh(bool _generateNormals = true);
	void LoadHeightmap(std::string _filepath);
	
	void LoadPerlinMap(float _scale);
	void SmoothHeights(int _iterations = 1);

	void SaveAsHeightmap();
	void CreateHeightmap() const;

	float Average(unsigned int x, unsigned int y);

	float SampleHeight(float _x, float _y);
	float SampleSteepness(float _x, float _y);

	float GetCellSpacing() const;
	glm::vec2 GetSize() const;

	void SetSeed(int _seed);

	float Smoothstep(float edge0, float edge1, float x);

	void Update()override;
	void Render()override;
};