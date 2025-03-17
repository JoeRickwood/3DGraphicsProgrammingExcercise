#pragma once
#include "ObjectInstance.h"


enum BlockType 
{
	Grass = 1, 
	Soil = 2,
	Stone = 3
};

class MapGenerator : public Component 
{
public:
	MapGenerator(glm::vec3 _tileSize, glm::vec3 _mapSize);
	~MapGenerator();

	void AddTile(glm::vec3 _position, BlockType _type);

	int SampleHeight(float x, float z);

	void Init()override;
	void Update()override;
	void Render()override;


	ObjectInstance* GetTile(glm::vec3 _position);

private:
	std::vector<ObjectInstance*> tiles;
	glm::vec3 tileSize;
	glm::vec3 mapSize;

	float frequency = 8.0f;
	int octaves = 8;
};