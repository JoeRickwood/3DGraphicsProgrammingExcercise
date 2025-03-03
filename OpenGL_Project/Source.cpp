#include "RenderableInstance.h"
#include "Components.h"

//Window Width And Height Of The Application (Pixels)
static int windowWidth = 800;
static int windowHeight = 800;

GLFWwindow* Window;

std::vector<RenderableInstance> renderableInstances;

void InitialSetup();
void Update();
void Render();
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height);

int main() 
{
	renderableInstances = 
	{
		RenderableInstance(RenderableType::Quad, glm::vec3(0.5f, 0.5f, 0.f), 45.f, glm::vec3(0.5f, 0.5f, 1.f)),
		RenderableInstance(RenderableType::Quad, glm::vec3(-0.5f, 0.5f, 0.f), 17.f, glm::vec3(0.5f, 0.5f, 1.f)),
	};

	renderableInstances[0].AddComponent<InputMove>();

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
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glViewport(0, 0, windowWidth, windowHeight);

	InputManager::Instance().Init();

	ShaderLoader::Instance().InitializeShaderPrograms();

	for (int i = 0; i < renderableInstances.size(); i++)
	{
		renderableInstances[i].Init();
	}
	

	glfwSetWindowSizeCallback(Window, (GLFWwindowsizefun)OnWindowResized);
}

void Update() 
{
	for (int i = 0; i < renderableInstances.size(); i++)
	{
		renderableInstances[i].Update();
	}

	glfwPollEvents();
}

void Render() 
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < renderableInstances.size(); i++)
	{
		renderableInstances[i].Render();
	}

	glfwSwapBuffers(Window);
}

void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height) 
{
	glViewport(0, 0, _Width, _Height);
}