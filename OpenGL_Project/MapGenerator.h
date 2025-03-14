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
	MapGenerator(glm::vec2 _tileSize, glm::vec2 _mapSize);
	~MapGenerator();

	void AddTile(glm::vec2 _position, BlockType _type);

	int SampleHeight(float x);

	void Init()override;
	void Update()override;
	void Render()override;


	ObjectInstance* GetTile(glm::vec3 _position);

private:
	std::vector<ObjectInstance*> tiles;
	glm::vec2 tileSize;
	glm::vec2 mapSize;
};