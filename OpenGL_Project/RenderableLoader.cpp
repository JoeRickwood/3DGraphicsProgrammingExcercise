#include "RenderableLoader.h"

void RenderableLoader::Init()
{
	//Values For The Hexagon Vertices 
	float opposite = 0.5f * sinf(3.141592653589793238462 / 6.0f);
	float adjacent = 0.5f * cosf(3.141592653589793238462 / 6.0f);

	//Set The Renderables Index Based On The RenderableType Enum
	//Renderable Instances Will Point To These Renderables To Render To The Screen
	//This Decreases Memory Usage Because Less Renderables Are Being Stored In Memory
	renderables = new Renderable[RenderableType::MAX]
	{
		Renderable( //Tri
		{
			//Position          // Color
			-0.5f, -0.25f, 0.0f,   1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
			0.5f, -0.25f, 0.0f,	 0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
			0.f, 0.25f, 0.0f,	 0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
		},

		{
			0, 1, 2, //First Triangle
		}
		),

		Renderable( //Quad
		{
			//Position          // Color            //Texture Coords
			-0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
			0.5f, 0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f
		},

		{
			0, 1, 2, //First Triangle
			1, 2, 3  //Second Triangle
		}
		),

		Renderable( //Hexagon
		{
			//Position                   // Color
			0.0f, 0.0f, 0.0f,            1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
			0.0f, 0.5f, 0.0f,	         1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
			-adjacent, opposite, 0.0f,	 1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
			-adjacent, -opposite, 0.0f,  0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
			0.0f, -0.5f, 0.0f,           0.0f, 1.0f, 1.0f,		0.0f, 1.0f,
			adjacent, -opposite, 0.0f,   0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
			adjacent, opposite, 0.0f,    1.0f, 0.0f, 1.0f,		0.0f, 1.0f,
		},

		{
			0, 1, 2,  //First Triangle
			0, 2, 3,  //Second Triangle
			0, 3, 4,  //Third Triangle
			0, 4, 5,  //Fourth Triangle
			0, 5, 6,  //Fifth Triangle
			0, 6, 1,  //Sixth Triangle
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
