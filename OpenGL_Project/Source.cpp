#include "Scene.h"
#include "Physics.h"

//Forward Declare Functions For Later
void InitialSetup();
void Update();
void Render();
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height);

int main() 
{
	//We Initialize The Renderable Loader BEFORE We Set Our Rendereable Instances On Screen
	//This Is To Prevent The Renderable Loader Returning "Default" Renderables (Quads)
	RenderableLoader::Instance().Init();

	ObjectInstance* test = new ObjectInstance("Test", glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.f), glm::vec3(100.f, 100.f, 100.f));

	test->AddComponent<Renderer>(RenderableType::Quad, ShaderType::Texture, 0, ProjectionType::Orthographic);
	Animator* anim = test->AddComponent<Animator>(3.f);
	anim->AddAnimation(Animation(0, 8, 1, 8));
	test->AddComponent<CameraController>(250.0f);

	Scene::Current().AddObject(test);

	//Initialize GLFW And setting the version to 4.6
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//Create Window
	GraphicsLoader::Instance().currentWindow = glfwCreateWindow(800, 800, "OPEN GL EXCERCISE", NULL, NULL);

	if(GraphicsLoader::Instance().currentWindow == NULL)
	{
		std::cout << "Error Creating GraphicsLoader::Instance().currentWindow, GLFW Failed To Initialize, Terminating Program" << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	//Make The Window The Current Context Of GLFW
	glfwMakeContextCurrent(GraphicsLoader::Instance().currentWindow);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLFW Failed To Initialize, Terminating Program" << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	//Setup All Objects In Project
	InitialSetup();
	RenderableLoader::Instance().LinkRenderables();

	//Application Loop Runs Until The Window Is Set To close
	while (glfwWindowShouldClose(GraphicsLoader::Instance().currentWindow) == false)
	{
		Time::Instance().Update();

		Update();

		Physics::Instance().ResolveCollisions();

		Camera::CalculateProjectionMatrix();
		Camera::CalculateViewMatrix();

		Render();
	}

	glfwTerminate();
	return 0;
}

//Sets Up Objects + Other GLFW Parameters
void InitialSetup() 
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_CULL_FACE);

	glClearColor(0.6f, 0.6f, 1.0f, 1.0f);
	glViewport(0, 0, GraphicsLoader::Instance().windowSize.x, GraphicsLoader::Instance().windowSize.y);

	GraphicsLoader::Instance().InitializeShaderPrograms(); // Generates The Shader Programs To Be Used By Renderables
	GraphicsLoader::Instance().InitializeTextures(); //Generates The Textures Used

	Time::Instance().Init();

	glfwSetWindowSizeCallback(GraphicsLoader::Instance().currentWindow, (GLFWwindowsizefun)OnWindowResized);
}

//Update Is Called Once Every Frame BEFORE Render
void Update() 
{
	Scene::Current().Update();

	glfwPollEvents();
}

//Render Is Called After Update And Draws Objects To The Screen
void Render() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Scene::Current().Render();

	glfwSwapBuffers(GraphicsLoader::Instance().currentWindow);
}

//On Window Resized Callback Links To The glfwWindowSizefun
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height) 
{
	glViewport(0, 0, _Width, _Height);

	GraphicsLoader::Instance().windowSize.x = _Width;
	GraphicsLoader::Instance().windowSize.y = _Height;
}