#include <iostream>

#include "Scene.h"
#include "Time.h"
#include "PerlinNoise.h"

#include "RenderingPipeline.h"

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

//Creates All Objects In The Scene
void LoadScene()
{
	ObjectInstance* skybox = new ObjectInstance("Skybox", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.f, 0.f, 0.f));
	skybox->AddComponent<Skybox>("Skybox", "MainSkybox");
	skybox->AddComponent<DirectionalLight>(glm::vec3(0.8f, -1.0f, 0.4f), glm::vec3(1.5f, 1.5f, 1.5f), 1.0f); 

	ObjectInstance* cam = new ObjectInstance("Camera", glm::vec3(0.f, 0.f, 0.f));
	cam->AddComponent<PlayerController>(0.003f, 15);

	ObjectInstance* cube1 = new ObjectInstance("Cube1", glm::vec3(-2.f, -1.f, -5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	auto cube1Renderer = cube1->AddComponent<DefaultRenderer>("Default", ProjectionType::Perspective);
	cube1Renderer->SetMesh(AssetLoader::Instance().GetMesh("Cube"));
	cube1Renderer->AddTexturePass("Texture0", "Prototype", Texture2D, TilingType::Repeat);
	cube1Renderer->AddTexturePass("ShadowMap", "DepthMap", Texture2D, TilingType::ClampBorder);
	cube1Renderer->SetShadowRendering(true);
	cube1Renderer->SetRenderType(RenderFront);

	ObjectInstance* terrain = new ObjectInstance("Terrain", glm::vec3(0.f, 0.f, 0.f));
	auto terrainRenderer = terrain->AddComponent<Terrain>(ProjectionType::Perspective, 256, 256, 10.0f);
	terrainRenderer->SetTextureTiling(glm::vec2(1, 1));
	terrainRenderer->AddTexturePass("Texture0", "Prototype", Texture2D, TilingType::Repeat);
	terrainRenderer->AddTexturePass("ShadowMap", "DepthMap", Texture2D, TilingType::ClampBorder);
	terrainRenderer->SetShadowRendering(true);
	//terrainRenderer->LoadHeightMap("Resources/Heightmap0.raw");
	terrainRenderer->LoadPerlinMap();

	//Create Trees On The Terrain
	ObjectInstance* trees = new ObjectInstance("Trees", glm::vec3(0.f, 0.f, 0.f));
	auto treesRenderer = trees->AddComponent<InstancedRenderer>("Default", ProjectionType::Perspective);
	treesRenderer->SetMesh(AssetLoader::Instance().GetMesh("Tree"));
	treesRenderer->AddTexturePass("Texture0", "Tree", Texture2D, TilingType::ClampEdges);
	treesRenderer->AddTexturePass("ShadowMap", "DepthMap", Texture2D, TilingType::ClampBorder);
	treesRenderer->SetShadowRendering(true);

	float treeSpacing = 2.75f;
	int treeGridX = 100;
	int treeGridY = 100;
	for (int x = 0; x < treeGridX; x++)
	{
		for (int y = 0; y < treeGridY; y++)
		{
			float t = ValueNoise_2D(x * 15.f, y * 15.f);
			
			float offsetX = (((rand() % 100) / 100.f) - 0.5f) * 3;
			float offsetY = (((rand() % 100) / 100.f) - 0.5f) * 3;
			float randScale = ((rand() % 100) / 100.f) + 0.5f;

			if (t >= 0.15f) 
			{
				float height = terrain->GetComponent<Terrain>()->SampleHeight(x * treeSpacing + offsetX, y * treeSpacing + offsetY);

				treesRenderer->AddInstance(glm::vec3(x * treeSpacing + offsetX, height, y * treeSpacing + offsetY), glm::vec3(), glm::vec3(randScale, randScale, randScale) * 0.3f);
			}
		}
	}

	treesRenderer->InitVBO();

	Scene::Current().SetAmbientLightStength(0.2f);
	Scene::Current().SetAmbientLightColor(glm::vec3(1.0f, 1.0f, 1.0f));

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

	ObjectInstance* UIObjectTest = new ObjectInstance("UIObjectTest", glm::vec3(50.f, 50.f, -1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	auto UIRenderer = UIObjectTest->AddComponent<TextRenderer>("DefaultText", ProjectionType::Orthographic);
	UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
	UIRenderer->SetFont("AldotheApache");
	UIRenderer->SetColor(glm::vec3(1.f, 1.f, 1.f));
	UIRenderer->SetText("Terrain Scene Test 1");
	UIRenderer->SetRenderType(RenderBoth);

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


	RenderingPipeline::InitShadowRendering();

	//Application Loop Runs Until The Window Is Set To close
	while (glfwWindowShouldClose(AssetLoader::Instance().currentWindow) == false)
	{
		Time::Instance().Update();

		Camera::CalculateProjectionMatrix();
		Camera::CalculateViewMatrix();

		Update();

		RenderingPipeline::ShadowPass();

		RenderingPipeline::Render();
	}

	glfwTerminate();
	return 0;
}