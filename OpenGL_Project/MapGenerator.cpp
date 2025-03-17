#include "MapGenerator.h"
#include "Collider.h"
#include "Renderer.h"
#include "Block.h"

#include "PerlinNoise.h"

MapGenerator::MapGenerator(glm::vec3 _tileSize, glm::vec3 _mapSize)
{
	tileSize = _tileSize;
	mapSize = _mapSize;
}

MapGenerator::~MapGenerator()
{

}

void MapGenerator::AddTile(glm::vec3 _position, BlockType _type)
{
	ObjectInstance* tile = new ObjectInstance("Tile " + tiles.size(), glm::vec3(_position.x * tileSize.x, _position.y * tileSize.y, _position.z * tileSize.z), glm::vec3(0.f), glm::vec3(tileSize.x, tileSize.y, tileSize.z));
	tile->AddComponent<Renderer>(RenderableType::Cube, ShaderType::Texture, _type);
	tile->AddComponent<Collider>();
	tile->AddComponent<Block>();

	tiles.push_back(tile);
}

int MapGenerator::SampleHeight(float x, float z)
{
	return 0;
}

void MapGenerator::Init()
{
	for (int i = 0; i < mapSize.z; i++)
	{
		for (int j = 0; j < mapSize.x; j++)
		{
			float sample = ValueNoise_2D(i * 1.f, j * 1.f) * 30.f;

			for (int k = 0; k < sample; k++)
			{
				glm::vec3 pos = glm::vec3(j + parent->position.x, k + parent->position.y, -i + parent->position.z);

				if (k >= sample - 1)
				{
					AddTile(pos, BlockType::Grass);
				}
				else if (k > sample - 3)
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
			return tiles[i];
		}
	}

	return nullptr;
}
