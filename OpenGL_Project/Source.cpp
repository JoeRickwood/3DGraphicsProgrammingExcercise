#include <iostream>

#include "Scene.h"
#include "Time.h"
#include "PerlinNoise.h"

//On Window Resized Callback Links To The glfwWindowSizefun
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height)
{
	glViewport(0, 0, _Width, _Height);

	GraphicsLoader::Instance().windowSize.x = (float)_Width;
	GraphicsLoader::Instance().windowSize.y = (float)_Height;
}

//Sets Up Objects + Other GLFW Parameters
void InitialSetup() 
{
	//Initialize GLFW And setting the version to 4.6
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_SAMPLES, 4);

	//Create Window
	GraphicsLoader::Instance().currentWindow = glfwCreateWindow((int)GraphicsLoader::Instance().windowSize.x, (int)GraphicsLoader::Instance().windowSize.y, "OPEN GL EXCERCISE", NULL, NULL);

	if (GraphicsLoader::Instance().currentWindow == NULL)
	{
		std::cout << "Error Creating GraphicsLoader::Instance().currentWindow, GLFW Failed To Initialize, Terminating Program" << std::endl;

		glfwTerminate();
		return;
	}

	//Make The Window The Current Context Of GLFW
	glfwMakeContextCurrent(GraphicsLoader::Instance().currentWindow);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLFW Failed To Initialize, Terminating Program" << std::endl;
		system("pause");

		glfwTerminate();
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);

	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	glfwSwapInterval(0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, (GLsizei)GraphicsLoader::Instance().windowSize.x, (GLsizei)GraphicsLoader::Instance().windowSize.y);
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

//Creates All Objects In The Scene
void LoadScene()
{
	ObjectInstance* skybox = new ObjectInstance("Skybox", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.f, 0.f, 0.f));
	skybox->AddComponent<Skybox>(0, ShaderType::Cubemap);

	ObjectInstance* cam = new ObjectInstance("Camera");
	cam->AddComponent<PlayerController>(2, 15);

	//Object Which Shows What A Regular Object With Animation Looks Like
	ObjectInstance* ground = new ObjectInstance("Ground", glm::vec3(0.0f, -10.f, 0.0f), glm::vec3(0.f), glm::vec3(100.f, 10.f, 100.f));
	auto groundRenderer = ground->AddComponent<DefaultRenderer>(ShaderType::Texture, ProjectionType::Perspective);
	groundRenderer->SetMesh(MeshLoader::Instance().GetMesh(1));
	groundRenderer->AddTexture("Texture0", 0, Texture2D);
	groundRenderer->SetTextureTiling(glm::vec2(25, 25));

	ObjectInstance* instancedCubes = new ObjectInstance("Instanced Cubes");
	auto cubesRenderer = instancedCubes->AddComponent<InstancedRenderer>(ShaderType::Instanced, ProjectionType::Perspective);
	cubesRenderer->SetMesh(MeshLoader::Instance().GetMesh(2));
	cubesRenderer->AddTexture("Texture0", 1, Texture2D);

	//Add All Objects To The Current Scene
	Scene::Current().AddObject(skybox);
	Scene::Current().AddObject(ground);
	Scene::Current().AddObject(instancedCubes);
	Scene::Current().AddObject(cam);

	for (int i = 0; i < 500; ++i)
	{
		glm::vec3 pos = glm::vec3((rand() % 200) - 100, 0.f, (rand() % 200) - 100);
		glm::vec3 rot = glm::vec3(0.0f, rand() % 360, 0.0f);
		glm::vec3 scale = glm::vec3(3.0f, 3.0f, 3.0f) * (((float)(rand() % 100) / 100.f) + 0.5f);

		cubesRenderer->AddInstance(pos, rot, scale);
	}

	cubesRenderer->InitInstancing();


	//Set Up Lights
	Scene::Current().AddPointLight(PointLight(
		glm::vec3(25.f, 15.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f,
		0.0027f,
		0.0028f
	));

	Scene::Current().AddPointLight(PointLight(
		glm::vec3(-25.0f, 15.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, .0f),
		10.0f
	));
	
	Scene::Current().AddSpotLight(SpotLight(
		glm::vec3(0.f, 10.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(1.f, 1.f, 1.f),
		45.f,
		50.f
	));

	Scene::Current().SetDirectionalLight(DirectionalLight(
		glm::vec3(0.3f, -1.f, 0.3f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.f
	));
}

int main()
{
	//Setup All Objects In Project
	InitialSetup();

	//We Initialize The Renderable Loader BEFORE We Set Our Rendereable Instances On Screen
	//This Is To Prevent The Renderable Loader Returning "Default" Renderables (Quads)
	MeshLoader::Instance().Init();
	MeshLoader::Instance().LinkMeshes();

	GraphicsLoader::Instance().InitializeShaderPrograms(); // Generates The Shader Programs To Be Used By Renderables
	GraphicsLoader::Instance().InitializeTextures(); //Generates The Textures Used

	Time::Instance().Init();

	LoadScene();

	//Application Loop Runs Until The Window Is Set To close
	while (glfwWindowShouldClose(GraphicsLoader::Instance().currentWindow) == false)
	{
		Time::Instance().Update();

		Camera::CalculateProjectionMatrix();
		Camera::CalculateViewMatrix();

		Update();

		Render();
	}

	glfwTerminate();
	return 0;
}