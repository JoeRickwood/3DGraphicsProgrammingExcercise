#include <iostream>

#include "Scene.h"
#include "Time.h"
#include "PerlinNoise.h"

//On Window Resized Callback Links To The glfwWindowSizefun
void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height)
{
	glViewport(0, 0, _Width, _Height);

	AssetLoader::Instance().windowSize.x = (float)_Width;
	AssetLoader::Instance().windowSize.y = (float)_Height;
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
	AssetLoader::Instance().currentWindow = glfwCreateWindow((int)AssetLoader::Instance().windowSize.x, (int)AssetLoader::Instance().windowSize.y, "OPEN GL EXCERCISE", NULL, NULL);

	if (AssetLoader::Instance().currentWindow == NULL)
	{
		std::cout << "Error Creating AssetLoader::Instance().currentWindow, GLFW Failed To Initialize, Terminating Program" << std::endl;

		glfwTerminate();
		return;
	}

	//Make The Window The Current Context Of GLFW
	glfwMakeContextCurrent(AssetLoader::Instance().currentWindow);

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
	glViewport(0, 0, (GLsizei)AssetLoader::Instance().windowSize.x, (GLsizei)AssetLoader::Instance().windowSize.y);
	glfwSetWindowSizeCallback(AssetLoader::Instance().currentWindow, (GLFWwindowsizefun)OnWindowResized);
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

	glfwSwapBuffers(AssetLoader::Instance().currentWindow);
}

//Creates All Objects In The Scene
void LoadScene()
{
	ObjectInstance* skybox = new ObjectInstance("Skybox", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.f, 0.f, 0.f));
	skybox->AddComponent<Skybox>("Skybox", "MainSkybox");
	skybox->AddComponent<DirectionalLight>(glm::vec3(0.4f, -1.0f, 0.4f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

	ObjectInstance* cam = new ObjectInstance("Camera", glm::vec3(0.f, 10.f, 0.f));
	cam->AddComponent<PlayerController>(0.003f, 15);
	cam->AddComponent<SpotLight>(glm::vec3(1.0f, 1.0f, 1.0f), 15.0f, 18.0f, 250.0f);

	//Object Which Shows What A Regular Object With Animation Looks Like
	ObjectInstance* ground = new ObjectInstance("Ground", glm::vec3(0.0f, -10.f, 0.0f), glm::vec3(0.f), glm::vec3(250.f, 10.f, 250.f));
	auto groundRenderer = ground->AddComponent<DefaultRenderer>("Default", ProjectionType::Perspective);
	groundRenderer->SetMesh(AssetLoader::Instance().GetMesh("Cube"));
	groundRenderer->AddTexture("Texture0", "Prototype", Texture2D);
	groundRenderer->SetTextureTiling(glm::vec2(25, 25));

	ObjectInstance* instancedGrass = new ObjectInstance("Grass");
	auto grassRenderer = instancedGrass->AddComponent<InstancedRenderer>("Grass", ProjectionType::Perspective);
	grassRenderer->SetMesh(AssetLoader::Instance().GetMesh("Grass"));
	grassRenderer->AddTexture("Texture0", "Grass", Texture2D);
	grassRenderer->SetRenderType(RenderBoth);

	ObjectInstance* instancedTrees = new ObjectInstance("Trees");
	auto treesRenderer = instancedTrees->AddComponent<InstancedRenderer>("DefaultInstanced", ProjectionType::Perspective);
	treesRenderer->SetMesh(AssetLoader::Instance().GetMesh("Tree"));
	treesRenderer->AddTexture("Texture0", "Tree", Texture2D);

	//Add All Objects To The Current Scene
	Scene::Current().AddObject(cam);

	Scene::Current().AddObject(skybox);
	Scene::Current().AddObject(ground);
	Scene::Current().AddObject(instancedGrass);
	Scene::Current().AddObject(instancedTrees);

	Scene::Current().SetAmbientLightStength(0.2f);
	Scene::Current().SetAmbientLightColor(glm::vec3(1.0f, 1.0f, 1.0f));

	for (int i = 0; i < 500000; ++i)
	{
		glm::vec3 pos = glm::vec3(ValueNoise_2D(i + 2312, i - 23712) * 750.f, 0.f, ValueNoise_2D(i - 232312, i + 23712) * 750.f);
		glm::vec3 rot = glm::vec3(0.0f, rand() % 360, 0.0f);
		glm::vec3 scale = glm::vec3(0.5f, 0.5f, 0.5f) * (((float)(rand() % 100) / 100.f) + 0.5f);

		grassRenderer->AddInstance(pos, rot, scale);
	}

	for (int i = 0; i < 500; ++i)
	{
		glm::vec3 pos = glm::vec3(ValueNoise_2D(i + 2312, i - 23712) * 500.f, 0.f, ValueNoise_2D(i - 232312, i + 23712) * 500.f);
		glm::vec3 rot = glm::vec3(0.0f, rand() % 360, 0.0f);
		glm::vec3 scale = glm::vec3(3.0f, 3.0f, 3.0f) * (((float)(rand() % 100) / 100.f) + 0.5f);

		treesRenderer->AddInstance(pos, rot, scale);
	}

	grassRenderer->InitInstancing();
	treesRenderer->InitInstancing();

	std::string skyboxPaths[6] =
	{
		"Resources/Skybox/Front.png",
		"Resources/Skybox/Back.png",
		"Resources/Skybox/Top.png",
		"Resources/Skybox/Bottom.png",
		"Resources/Skybox/Right.png",
		"Resources/Skybox/Left.png"
	};

	AssetLoader::CreateSkybox(skyboxPaths, "MainSkybox");
}

int main()
{
	//Setup All Objects In Project
	InitialSetup();

	//We Initialize The Renderable Loader BEFORE We Set Our Object Instances On Screen
	//This Is To Prevent The Renderable Loader Returning "Default" Renderables (None)
	AssetLoader::Instance().LoadAssets("Resources");

	Time::Instance().Init();

	LoadScene();

	//Application Loop Runs Until The Window Is Set To close
	while (glfwWindowShouldClose(AssetLoader::Instance().currentWindow) == false)
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