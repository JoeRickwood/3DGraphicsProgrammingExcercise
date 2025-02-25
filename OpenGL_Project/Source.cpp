#include "ShaderLoader.h"

//Window Width And Height Of The Application (Pixels)
static int windowWidth = 800;
static int windowHeight = 800;

GLFWwindow* Window;

GLfloat Vertices_Tri[] = {
	//Position          // Color
	-0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,
};

GLuint VAO_Tri;


void InitialSetup();
void Update();
void Render();
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height);

int main() 
{
	//Initialize GLFW And setting the version to 4.6
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//Create Window
	Window = glfwCreateWindow(windowWidth, windowHeight, "OPEN GL EXCERCISE", NULL, NULL);

	if(Window == NULL) 
	{
		std::cout << "Error Creating Window, GLFW Failed To Initialize, Terminating Program" << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(Window);


	if (glewInit() != GLEW_OK) 
	{
		std::cout << "GLFW Failed To Initialize, Terminating Program" << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}



	InitialSetup();

	while (glfwWindowShouldClose(Window) == false) 
	{
		Update();

		Render();
	}


	glfwTerminate();
	return 0;
}

void InitialSetup() 
{
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glViewport(0, 0, windowWidth, windowHeight);

	ShaderLoader::Instance().InitializeShaderPrograms();

	GLuint VBO_Tri;

	//Generate The VAO For A Triangle
	glGenVertexArrays(1, &VAO_Tri);
	glBindVertexArray(VAO_Tri);

	//Generate The VBO For A Triangle
	glGenBuffers(1, &VBO_Tri);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Tri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Tri), Vertices_Tri, GL_STATIC_DRAW);

	//Set Vertex Attribute Info, This Is How To Interpret The Vertex Data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);



	glfwSetWindowSizeCallback(Window, (GLFWwindowsizefun)OnWindowResized);
}

void Update() 
{
	glfwPollEvents();
}

void Render() 
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(ShaderLoader::Instance().GetShaderProgram(0));
	glBindVertexArray(VAO_Tri);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glUseProgram(0); 

	glfwSwapBuffers(Window);
}

void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height) 
{
	glViewport(0, 0, _Width, _Height);
}