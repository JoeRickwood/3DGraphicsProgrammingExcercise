#include "ShaderLoader.h"

//Window Width And Height Of The Application (Pixels)
static int windowWidth = 800;
static int windowHeight = 800;

GLFWwindow* Window;
GLuint Program_FixedTri = NULL;

float sinX;
float sinY;
float sinZ;
float timer;

void InitialSetup();
void Update();
void Render();
void OnWindowResized();

int main() 
{
	//Initialize GLFW And setting the version to 4.6
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
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

		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

void InitialSetup() 
{
	glClearColor(sinX, sinY, sinZ, 1.0f);

	glViewport(0, 0, windowWidth, windowHeight);


	Program_FixedTri = ShaderLoader::CreateProgram("Resources/Shaders/FixedTriangle.vert",
													"Resources/Shaders/FixedColor.frag");

	glfwSetWindowSizeCallback(Window, OnWindowResized);


	//glPolygonMode(GL_FRONT, GL_POINT);
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_FRONT, GL_FILL);

	//glfwSetWindowOpacity(Window, 0.5f);
}

void Update() 
{
	timer += (1.f / 60.f);

	sinX = (std::sin(timer) + 1.f) / 2.f;
	sinY = (std::sin(timer + 2317) + 1.f) / 2.f;
	sinZ = (std::sin(timer - 83213) + 1.f) / 2.f;

	glClearColor(sinX, sinY, sinZ, 1.0f);
}

void Render() 
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(Program_FixedTri);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glUseProgram(0); 

	glfwSwapBuffers(Window);
}

void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height) 
{
	glViewport(0, 0, _Width, _Height);
}