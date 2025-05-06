#include <iostream>

#include "Scene.h"
#include "Time.h"
#include "PerlinNoise.h"

//Forward Declare Functions For Later
void InitialSetup();
void Update();
void Render();
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height);

int main() 
{
	//We Initialize The Renderable Loader BEFORE We Set Our Rendereable Instances On Screen
	//This Is To Prevent The Renderable Loader Returning "Default" Renderables (Quads)
	MeshLoader::Instance().Init();

	//Initialize GLFW And setting the version to 4.6
	glfwInit();
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//Funni Transparent Window Stuff XD
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);

	//Object Which Shows What A Regular Object With Animation Looks Like
	ObjectInstance* ground = new ObjectInstance("Ground", glm::vec3(0.0f, -10.f, 0.0f), glm::vec3(0.f), glm::vec3(100.f, 10.f, 100.f));
	auto groundRenderer = ground->AddComponent<DefaultRenderer>(0, ShaderType::Texture, 0, ProjectionType::Perspective);
	groundRenderer->textureTiling = glm::vec2(20.f, 20.f);
	ground->AddComponent<Collider>(glm::vec3(200.f, 10.0f, 200.f), glm::vec3(0.f, 5.f, 0.f));

	ObjectInstance* tree = new ObjectInstance("Tree", glm::vec3(0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(2.f, 2.f, 2.f));
	auto treeRenderer = tree->AddComponent<InstancedRenderer>(1, ShaderType::Instanced, 1, ProjectionType::Perspective);
	auto treeCollider = tree->AddComponent<InstancedCollider>();

	ObjectInstance* grass = new ObjectInstance("Grass", glm::vec3(0.0f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f));
	auto grassRenderer = grass->AddComponent<InstancedRenderer>(2, ShaderType::GrassSway, 2, ProjectionType::Perspective);

	ObjectInstance* player = new ObjectInstance("Player", glm::vec3(0.f, 10.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f));
	player->AddComponent<PlayerController>(10.f, 2.f, 1.f);
	player->AddComponent<PhysicsObject>();
	player->AddComponent<Collider>(glm::vec3(0.5f, 2.f, 0.5f), glm::vec3(0.f, 0.f, 0.f));
	auto cc = player->AddComponent<CameraController>(0.1f, 150.f);
	cc->SetEnabledState(false);

	Scene::Current().AddObject(ground);
	Scene::Current().AddObject(tree);
	Scene::Current().AddObject(grass);
	Scene::Current().AddObject(player);


	//Create Window
	GraphicsLoader::Instance().currentWindow = glfwCreateWindow((int)GraphicsLoader::Instance().windowSize.x, (int)GraphicsLoader::Instance().windowSize.y, "OPEN GL EXCERCISE", NULL, NULL);

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

	MeshLoader::Instance().LinkMeshes();

	for (int i = 0; i < 500; ++i)
	{
		float x = Noise(3, i, i + 3434) * 100.f;
		float y = Noise(7, i - 231276, i + 3213) * 100.f;

		float scale = (float)(((rand() % 100) / 100.f) + 0.5f) * 5;
		float rot = (float)(rand() % 360);

		treeRenderer->AddInstance(glm::vec3(x, 0.f, y), glm::vec3(0.f, rot, 0.f), glm::vec3(scale, scale, scale));
		treeCollider->AddInstance(glm::vec3(1.f, 2.f, 1.f) * scale, glm::vec3(x, 0.f, y));
	}

	for (int i = 0; i < 500000; ++i)
	{
		float x = Noise(3, i, i + 3434) * 100.f;
		float y = Noise(7, i - 231276, i + 3213) * 100.f;

		float scale = (((rand() % 100) / 100.f) + 0.5f) * 0.5f;
		float rot = (float)(rand() % 360);

		grassRenderer->AddInstance(glm::vec3(x, 0.f, y), glm::vec3(0.f, rot, 0.f), glm::vec3(scale, scale * 1.5f, scale));
	}

	grassRenderer->InitInstancing();
	treeRenderer->InitInstancing();


	Scene::Current().ShaderInit();

	//Application Loop Runs Until The Window Is Set To close
	while (glfwWindowShouldClose(GraphicsLoader::Instance().currentWindow) == false)
	{
		Time::Instance().Update();

		std::cout << "Game Window (" + to_string((int)round(1.f / Time::Instance().deltaTime)) + " FPS) \n";

		Update();

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
	glEnable(GL_BLEND);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glfwSwapInterval(0);

	glClearColor(0.6f, 0.6f, 1.0f, 1.0f);
	glViewport(0, 0, (GLsizei)GraphicsLoader::Instance().windowSize.x, (GLsizei)GraphicsLoader::Instance().windowSize.y);

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

	GraphicsLoader::Instance().windowSize.x = (float)_Width;
	GraphicsLoader::Instance().windowSize.y = (float)_Height;
}