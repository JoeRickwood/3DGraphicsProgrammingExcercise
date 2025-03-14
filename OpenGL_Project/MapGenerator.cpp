#include "MapGenerator.h"
#include "Collider.h"
#include "Renderer.h"
#include "Block.h"


MapGenerator::MapGenerator(glm::vec2 _tileSize, glm::vec2 _mapSize)
{
	tileSize = _tileSize;
	mapSize = _mapSize;
}

MapGenerator::~MapGenerator()
{

}

void MapGenerator::AddTile(glm::vec2 _position, BlockType _type)
{
	ObjectInstance* tile = new ObjectInstance("Tile " + tiles.size(), glm::vec3(_position.x * tileSize.x, _position.y * tileSize.y, 0.f), glm::vec3(0.f), glm::vec3(tileSize.x, tileSize.y, 0.0f));
	tile->AddComponent<Renderer>(RenderableType::Quad, ShaderType::Texture, _type);
	tile->AddComponent<Collider>();
	tile->AddComponent<Block>();

	tiles.push_back(tile);
}

int MapGenerator::SampleHeight(float x)
{
	float noise0 = (sin(x / 10.f) + 1.f) / 2.f;
	float noise1 = (sin(x / 6.f) + 1.f) / 2.f;
	float noise2 = (sin(x / 25.f) + 1.f) * 2.f;
	float noise3 = (sin(x / 2.f) + 1.f) / 2.f;

	int baseLevel = 3;

	return ((noise0 + noise1 + noise2 + noise3) * 2.f) + baseLevel;
}

void MapGenerator::Init()
{
	for (int i = 0; i < mapSize.x; i++)
	{
		int sample = SampleHeight(i);

		for (int j = 0; j < sample; j++)
		{
			glm::vec2 pos = glm::vec2(i + parent->position.x, j + parent->position.y);

			if (j == sample - 1)
			{
				AddTile(pos, BlockType::Grass);
			}
			else if (j > sample - 3)
			{
				AddTile(pos, BlockType::Soil);
			}
			else
			{
				AddTile(pos, BlockType::Stone);
			}
		}

	}
}

void MapGenerator::Update()
{
	for (auto* tile : tiles)
	{
		tile->Update();
	}
}

void MapGenerator::Render()
{
	for (auto* tile : tiles)
	{
		tile->Render();
	}
}

ObjectInstance* MapGenerator::GetTile(glm::vec3 _position)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		if(tiles[i] == nullptr)
		{
			continue;
		}

		if (tiles[i]->GetComponent<Collider>() == nullptr)
		{
			continue;
		}

		if (tiles[i]->GetComponent<Collider>()->GetRect()->Contains(_position))
		{
			std::cout << tiles[i]->position.x << "," << tiles[i]->position.y << std::endl;
			return tiles[i];
		}
	}

	return nullptr;
}
