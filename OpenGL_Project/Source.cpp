#include <iostream>

#include "Scene.h"
#include "Time.h"
#include "MathFunctions.h"

#include "RenderingPipeline.h"

//On Window Resized Callback Links To The glfwWindowSizefun
static void OnWindowResized(GLFWwindow* _Window, int _Width, int _Height)
{
	glViewport(0, 0, _Width, _Height);

	AssetLoader::Instance().windowSize.x = (float)_Width;
	AssetLoader::Instance().windowSize.y = (float)_Height;
}

//Sets Up Objects + Other GLFW Parameters
static void InitialSetup() 
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


//Creates All Objects In The Scene1
static void LoadScene1()
{
	Scene::Current().ChangeScene(1);

	ObjectInstance* skybox = new ObjectInstance("Skybox", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.f, 0.f, 0.f));
	skybox->AddComponent<Skybox>("Skybox", "MainSkybox");
	skybox->AddComponent<DirectionalLight>(glm::vec3(0.8f, -0.8f, 0.4f), glm::vec3(1.5f, 1.5f, 1.5f), 1.0f); 

	ObjectInstance* cam = new ObjectInstance("Camera", glm::vec3(0.f, 0.f, 0.f));
	cam->AddComponent<PlayerController>(0.003f, 100);

	ObjectInstance* terrain = new ObjectInstance("Terrain", glm::vec3(0.f, 0.f, 0.f));
	auto terrainRenderer = terrain->AddComponent<Terrain>("DefaultTerrain", ProjectionType::Perspective, 512, 512, 1.0f);
	terrainRenderer->SetTextureTiling(glm::vec2(1, 1));
	terrainRenderer->AddTexturePass("TextureGrass", "Grass", Texture2D, TilingType::Repeat);
	terrainRenderer->AddTexturePass("TextureGrassVariant", "GrassVariant", Texture2D, TilingType::Repeat);
	terrainRenderer->AddTexturePass("TextureGrassVariationNoise", "GrassVariationNoise", Texture2D, TilingType::Repeat);
	terrainRenderer->AddTexturePass("TextureSand", "MuddySand", Texture2D, TilingType::Repeat);
	terrainRenderer->AddTexturePass("TextureRock", "Rock", Texture2D, TilingType::Repeat);
	terrainRenderer->AddTexturePass("TextureSnow", "Snow", Texture2D, TilingType::Repeat);
	terrainRenderer->AddTexturePass("ShadowMap", "DepthMap", Texture2D, TilingType::ClampBorder);
	terrainRenderer->AddTexturePass("TextureNoise", "Noise", Texture2D, TilingType::Repeat);

	terrainRenderer->SetShadowRendering(true);

	terrainRenderer->SetSeed(rand() % 100000);
	terrainRenderer->CreateHeightmap();
	terrainRenderer->LoadHeightmap("NoiseTextures/Noise.raw");
	terrainRenderer->SmoothHeights(2);
	terrainRenderer->GenerateMesh(true); 


	/*ObjectInstance* trees = new ObjectInstance("Trees", glm::vec3(0.0f, 0.0f, 0.0f));
	auto treesRenderer = trees->AddComponent<InstancedRenderer>("Default", ProjectionType::Perspective);
	treesRenderer->SetMesh(AssetLoader::Instance().GetMesh("Tree"));
	treesRenderer->AddTexturePass("Texture0", "Tree", Texture2D, TilingType::Repeat);
	treesRenderer->AddTexturePass("ShadowMap", "DepthMap", Texture2D, TilingType::ClampBorder);
	treesRenderer->SetShadowRendering(true);

	float treeSpacing = terrain->GetComponent<Terrain>()->GetCellSpacing();
	int treesGridX = 512;
	int treesGridY = 512;

	for (int x = 0; x < treesGridX; x++)
	{
		for (int y = 0; y < treesGridY; y++)
		{
			bool spawnTree = (rand() % 100 > 95) && (PerlinNoise(x * treeSpacing * 15, y * treeSpacing * 15) > 0.1f);

			if (!spawnTree) 
			{
				continue;
			}

			float height = terrainRenderer->SampleHeight(x, y);


			if (height < 1)
			{
				continue;
			}

			float steepness = terrainRenderer->SampleSteepness(x, y);

			if (steepness < 0.9f)
			{
				continue;
			}

			float scalar = abs(PerlinNoise(x * treeSpacing, y * treeSpacing)) * 5.0f;

			float rot = rand() % 360;

			treesRenderer->AddInstance(glm::vec3(x * treeSpacing, height, y * treeSpacing), glm::vec3(0.0f, rot, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) * scalar);
		}
	}

	ObjectInstance* grass = new ObjectInstance("Grass", glm::vec3(0.f, 0.f, 0.f));
	auto grassRenderer = grass->AddComponent<InstancedRenderer>("Grass", ProjectionType::Perspective);
	grassRenderer->SetMesh(AssetLoader::Instance().GetMesh("Grass"));
	grassRenderer->AddTexturePass("Texture0", "GrassBlade", Texture2D, Repeat);
	grassRenderer->AddTexturePass("ShadowMap", "DepthMap", Texture2D, ClampBorder);
	grassRenderer->SetShadowRendering(false);

	float grassSpacing = terrain->GetComponent<Terrain>()->GetCellSpacing();
	int grassGridX = 512;
	int grassGridY = 512;

	for (int x = 0; x < grassGridX; x++)
	{
		for (int y = 0; y < grassGridY; y++)
		{
			float height = terrainRenderer->SampleHeight(x, y);

			if (height < 2.5) 
			{
				continue;
			}

			float steepness = terrainRenderer->SampleSteepness(x, y);

			if (steepness < 0.9f) 
			{
				continue;
			}

			float rot = rand() % 360;

			grassRenderer->AddInstance(glm::vec3(x * grassSpacing, height, y * grassSpacing), glm::vec3(0.0f, rot, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		}
	} */

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
	auto UIRenderer = UIObjectTest->AddComponent<TextRenderer>("DefaultText", ProjectionType::ScreenOrthographic);
	UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
	UIRenderer->SetFont("AldotheApache");
	UIRenderer->SetColor(glm::vec3(1.f, 1.f, 1.f));
	UIRenderer->SetText("Terrain Scene Test 1");
	UIRenderer->SetRenderType(RenderBoth);

}

//Creates All Objects In The Scene2
static void LoadScene2()
{
	Scene::Current().ChangeScene(2);

	Camera::Instance().SetCameraPosition(glm::vec3(0.0f, 0.0f, -10.0f));
	Camera::Instance().SetCameraLookDirection(glm::vec3(0.0f, 0.0f, 1.0f));

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

	{
		UIObjectInstance* UIObjectTest = new UIObjectInstance("UIObjectTest", glm::vec3(0.f, -150.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(2.f, 2.f, 2.f));
		auto UIRenderer = UIObjectTest->AddComponent<TextRenderer>("DefaultText", ProjectionType::ScreenOrthographic);
		UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIRenderer->SetFont("AldotheApache");
		UIRenderer->SetColor(glm::vec3(1.f, 1.f, 1.f));
		UIRenderer->SetText("Perlin Noise Scene");
		UIRenderer->SetRenderType(RenderBoth);
		UIObjectTest->SetScreenAlignment(MIDDLE, TOP);
	} 

	{
		UIObjectInstance* UIObjectTest = new UIObjectInstance("UIObjectTest", glm::vec3(-250.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(100.f, 100.f, 100.f));
		auto UIRenderer = UIObjectTest->AddComponent<DefaultRenderer>("DefaultUI", ProjectionType::ScreenOrthographic);
		UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIRenderer->SetRenderType(RenderBoth);
		UIRenderer->AddTexturePass("Texture0", "Noise", Texture2D, Repeat);
		UIObjectTest->SetScreenAlignment(MIDDLE, CENTER);
	}

	{
		UIObjectInstance* UIObjectTest = new UIObjectInstance("UIObjectTest", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(100.f, 100.f, 100.f));
		auto UIRenderer = UIObjectTest->AddComponent<DefaultRenderer>("4WayGradient", ProjectionType::ScreenOrthographic);
		UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIRenderer->SetRenderType(RenderBoth);
		UIRenderer->AddTexturePass("Texture0", "Noise", Texture2D, Repeat);
		UIObjectTest->SetScreenAlignment(MIDDLE, CENTER);
	}

	{
		UIObjectInstance* UIObjectTest = new UIObjectInstance("UIObjectTest", glm::vec3(250.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(100.f, 100.f, 100.f));
		auto UIRenderer = UIObjectTest->AddComponent<DefaultRenderer>("BurnNoise", ProjectionType::ScreenOrthographic);
		UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIRenderer->SetRenderType(RenderBoth);
		UIRenderer->AddTexturePass("Texture0", "Noise", Texture2D, Repeat);
		UIObjectTest->SetScreenAlignment(MIDDLE, CENTER);
	}

	//Buttons
	{
		UIObjectInstance* UIObjectTest = new UIObjectInstance("UIObjectTest", glm::vec3(-275.f, 150.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(150.f, 75.f, 1.f));
		auto UIRenderer = UIObjectTest->AddComponent<DefaultRenderer>("BorderedUI", ProjectionType::ScreenOrthographic);
		UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIRenderer->SetRenderType(RenderBoth);
		UIRenderer->AddTexturePass("Texture0", "Button", Texture2D, Repeat);
		UIObjectTest->SetScreenAlignment(MIDDLE, BOTTOM);
		auto button = UIObjectTest->AddComponent<Button>();
		button->AddListener([] 
		{
			Scene::Current().ChangeScene(1);

			Terrain* terrain = Scene::Current().FindObject("Terrain")->GetComponent<Terrain>();

			terrain->SetSeed(2332);
			terrain->CreateHeightmap();

			Scene::Current().ChangeScene(2);

		});
		UIRenderer->SetBorderSize(50);
		UIRenderer->SetTextureSize(glm::vec2(64, 64));


		ObjectInstance* UITextObject = new ObjectInstance("UITextObjectTest", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.0f, 1.0f, 1.0f));
		auto UITextRenderer = UITextObject->AddComponent<TextRenderer>("DefaultText", ProjectionType::ScreenOrthographic);
		UITextRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UITextRenderer->SetFont("AldotheApache");
		UITextRenderer->SetColor(glm::vec3(1.f, 1.f, 1.f));
		UITextRenderer->SetText("New Heightmap");
		UITextRenderer->SetRenderType(RenderBoth);

		UITextObject->SetParent(UIObjectTest);
		UITextObject->SetPosition(glm::vec3(0.f, 0.f, 0.f));
	}

	{
		UIObjectInstance* UIObjectTest = new UIObjectInstance("UIObjectTest", glm::vec3(275.f, 150.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(150.f, 75.f, 1.f));
		auto UIRenderer = UIObjectTest->AddComponent<DefaultRenderer>("BorderedUI", ProjectionType::ScreenOrthographic);
		UIRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UIRenderer->SetRenderType(RenderBoth);
		UIRenderer->AddTexturePass("Texture0", "Button", Texture2D, Repeat);
		UIObjectTest->SetScreenAlignment(MIDDLE, BOTTOM);
		auto button = UIObjectTest->AddComponent<Button>();
		button->AddListener([]
			{
				Scene::Current().ChangeScene(1);

				Terrain* terrain = Scene::Current().FindObject("Terrain")->GetComponent<Terrain>();

				terrain->LoadHeightmap("NoiseTextures/Noise.raw");
				terrain->SmoothHeights(2);
				terrain->GenerateMesh(true);

				Scene::Current().ChangeScene(2);

			});
		UIRenderer->SetBorderSize(50);
		UIRenderer->SetTextureSize(glm::vec2(64, 64));


		ObjectInstance* UITextObject = new ObjectInstance("UITextObjectTest", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.0f, 1.0f, 1.0f));
		auto UITextRenderer = UITextObject->AddComponent<TextRenderer>("DefaultText", ProjectionType::ScreenOrthographic);
		UITextRenderer->SetMesh(AssetLoader::Instance().GetMesh("Quad"));
		UITextRenderer->SetFont("AldotheApache");
		UITextRenderer->SetColor(glm::vec3(1.f, 1.f, 1.f));
		UITextRenderer->SetText("Apply To Terrain");
		UITextRenderer->SetRenderType(RenderBoth);

		UITextObject->SetParent(UIObjectTest);
		UITextObject->SetPosition(glm::vec3(0.f, 0.f, 0.f));
	}

}

int main()
{
	bool pressLock = false;

	//Setup All Objects In Project
	InitialSetup();

	//We Initialize The Renderable Loader BEFORE We Set Our Object Instances On Screen
	//This Is To Prevent The Renderable Loader Returning "Default" Renderables (None)
	AssetLoader::Instance().LoadAssets("Resources");

	Time::Instance().Init();

	LoadScene1();
	LoadScene2();

	RenderingPipeline::InitShadowRendering();

	//Application Loop Runs Until The Window Is Set To close
	while (glfwWindowShouldClose(AssetLoader::Instance().currentWindow) == false)
	{
		Time::Instance().Update();

		Camera::CalculateProjectionMatrix();
		Camera::CalculateViewMatrix();

		//Loads Scene 1
		if (pressLock == false && glfwGetKey(AssetLoader::Instance().currentWindow, GLFW_KEY_1) == GLFW_PRESS)
		{
			pressLock = true;
			Scene::Current().ChangeScene(1);
		}

		//Loads Scene 2
		if (pressLock == false && glfwGetKey(AssetLoader::Instance().currentWindow, GLFW_KEY_2) == GLFW_PRESS)
		{
			pressLock = true;
			Scene::Current().ChangeScene(2);
		}


		//Reset Press Lock State
		if (glfwGetKey(AssetLoader::Instance().currentWindow, GLFW_KEY_1) == GLFW_RELEASE ||
			glfwGetKey(AssetLoader::Instance().currentWindow, GLFW_KEY_2) == GLFW_RELEASE)
		{
			pressLock = false;
		}


		Scene::Current().Update();

		glfwPollEvents();

		RenderingPipeline::ShadowPass();
		RenderingPipeline::Render();
	}

	glfwTerminate();
	return 0;
}