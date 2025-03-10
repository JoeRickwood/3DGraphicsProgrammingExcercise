#pragma once
#include "GraphicsLoader.h"

//This Tells The RenderableInstance What Renderable To Use At Its "Stamp"
enum RenderableType : int
{
	Triangle = 0,
	Quad = 1,
	Hexagon = 2,
	MAX = 3 //MAX RenderableType Not Used
};

class Renderer;

class Renderable
{
private: //Renderables Contain The Base Data To Be Drawn to The Screen, Used As Basically A "Stamp" Which Multiple Instances Can Use
	std::vector<GLfloat> data = {
		//Position          // Color
		-0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f
	};

	//Indices Tell The Program Which Vertices To Use For Each Triangle
	std::vector<GLuint> indices = {
		0, 1, 2, //First Triangle
		0, 2, 3  //Second Triangle
	};
	
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	bool initialized;

public:
	Renderable();
	Renderable(std::vector<GLfloat> _data, std::vector<GLuint> _indices);
	~Renderable();

	void Init();
	void Draw();

	friend class Renderer;

};

