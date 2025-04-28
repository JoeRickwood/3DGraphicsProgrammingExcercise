#include "RenderableLoader.h"

void RenderableLoader::Init()
{
	//Values For The Hexagon Vertices 
	float opposite = 0.5f * sinf(3.1415f / 6.0f);
	float adjacent = 0.5f * cosf(3.1415f / 6.0f);

	//Set The Renderables Index Based On The RenderableType Enum
	//Renderable Instances Will Point To These Renderables To Render To The Screen
	//This Decreases Memory Usage Because Less Renderables Are Being Stored In Memory
	renderables = new Renderable[RenderableType::MAX]
	{
		Renderable( //Quad
		{
			//Position           //Texture Coords
			-0.5f, 0.5f, 0.0f,   0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f,	 0.0f, 0.0f,
			0.5f, 0.5f, 0.0f,	 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f,   1.0f, 0.0f
		},

		{
			0, 1, 2, //First Triangle
			3, 2, 1  //Second Triangle
		}
		),

		Renderable( //Cube
		{
			//Position                   // Texture Coord
			-0.5f, 0.5f, 0.5f,          0.0f, 1.0f, //Front
			-0.5f, -0.5f, 0.5f,	        0.0f, 0.0f,
			0.5f, -0.5, 0.5f,			1.0f, 0.0f,
			0.5f, 0.5, 0.5f,			1.0f, 1.0f,

			0.5f,  0.5f, -0.5f,         0.0f, 1.0f, //Back
			0.5f, -0.5f, -0.5f,			0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,		1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f,			1.0f, 1.0f,

			0.5f, 0.5f, 0.5f,			0.0f, 1.0f, //Right
			0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,			1.0f, 0.0f,
			0.5f, 0.5f, -0.5f,			1.0f, 1.0f,

			-0.5f, 0.5f, -0.5f,			0.0f, 1.0f, //Left
			-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,		1.0f, 0.0f,
			-0.5f, 0.5f,  0.5f,			1.0f, 1.0f,

			-0.5f, 0.5f, -0.5f,			0.0f, 1.0f, //Top
			-0.5f, 0.5f,  0.5f,			0.0f, 0.0f,
			0.5f, 0.5f,  0.5f,			1.0f, 0.0f,
			0.5f, 0.5f, -0.5f,			1.0f, 1.0f,

			-0.5f, -0.5f, 0.5f,			0.0f, 1.0f, //Bottom
			-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,			1.0f, 0.0f,
			0.5f, -0.5f, 0.5f,			1.0f, 1.0f
		},

		{
			0, 1, 2, //0
			0, 2, 3, //1
			4, 5, 6, //2
			4, 6, 7, //3
			8, 9, 10, //4
			8, 10, 11, //5
			12, 13, 14, //6
			12, 14, 15, //7
			16, 17, 18, //8
			16, 18, 19, //9
			20, 21, 22, //10
			20, 22, 23 //11
		}
		),
	};
}

void RenderableLoader::LinkRenderables()
{
	for (int i = 0; i < RenderableType::MAX; i++)
	{
		renderables[i].Init();
	}
}

Renderable* RenderableLoader::GetRenderable(RenderableType _type)
{
	return &renderables[(int)_type]; //Returns Renderable Based On The RenderableType Enum
}

RenderableLoader::RenderableLoader() 
{

}

RenderableLoader::~RenderableLoader()
{

}
