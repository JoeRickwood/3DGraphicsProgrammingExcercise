#include "ShaderLoader.h"

static int windowWidth = 800;
static int windowHeight = 800;

GLFWwindow* Window;
GLuint Program_FixedTri = NULL;

void InitialSetup();
void Update();
void Render();

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
	}


	glfwTerminate();
	return 0;
}

void InitialSetup() 
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	glViewport(0, 0, windowWidth, windowHeight);


	//Program_FixedTri = ShaderLoader::CreateProgram("Resources/Shaders/FixedTriangle.vert",
													"Resources/Shaders/FixedColor.frag");
}

void Update() 
{

}

void Render() 
{
	glClear(GL_COLOR_BUFFER_BIT);

	//glUseProgram(Program_FixedTri);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glUseProgram(0); 

	glfwSwapBuffers(Window);
}