#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MoonEngine.h"
#include "LevelEditor/LevelLoader.h"
#include "LevelEditor/LevelBuilder.h"

using namespace MoonEngine;


int main(int argc, char **argv) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    float windowWidth = 854.0f, windowHeight = 480.0f;
	GLFWwindow * window = glfwCreateWindow(windowWidth, windowHeight, "LevelEditor", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	Logger::SetLogLevel(INFO);


	std::shared_ptr<EngineApp> app = std::make_shared<EngineApp>(window, "moonengine.cfg");
	Scene * scene = new Scene();

    std::shared_ptr<GameObject> cameraObj = std::make_shared<GameObject>();

    Camera * cam = scene->createComponent<Camera>( 3.1415/3, 800.0/600.0, 0.1, 1000);
	FirstPersonController * ctrl = scene->createComponent<FirstPersonController>(5);
	cameraObj->addComponent(ctrl);
	cameraObj->addComponent(cam);
	cameraObj->addTag(T_Player);
	cameraObj->getTransform().translate(glm::vec3(0,150,-5));

	Component * record = scene->createComponent<CameraRecorder>();
	cameraObj->addComponent(record);

	scene->addGameObject(cameraObj);


	CDLODQuadtree::CreateInfo createInfo;
	//ImplicitHeightmapSource heightSource(256,256,[](int, int){return 0;});
	TextureHeightmapSource texSource("resources/textures","grandCanyon",".png");
	createInfo.source = &texSource;
	createInfo.leafNodeSize = 1;
	createInfo.LODLevelCount = 5;
	MapDimensions mapDims;
	
	mapDims.size = glm::vec3(1000,200,1000);
	mapDims.minCoords = glm::vec3(0,0,0);	
	mapDims.minCoords = -mapDims.size/2.0f;
	mapDims.minCoords.y = 0;
	createInfo.dimensions = mapDims;
	std::shared_ptr<GameObject> terrainObject = std::make_shared<GameObject>(Transform());
	terrainObject->addComponent(scene->createComponent<Terrain>(createInfo));

	std::shared_ptr<GameObject> dirLight = make_shared<GameObject>();
    dirLight->addComponent(scene->createComponent<DirLight>(glm::vec3(-1, -1, -1), COLOR_WHITE, 0.1f, 0.5f));
    scene->addGameObject(dirLight);

	//Preload canyon 32f texture
	EngineApp::GetAssetLibrary().TextureLib->createImage("grandCanyon",".png",true);
	
	 stringmap canyon_texture(
            {{"heightmap", "grandCanyon"},
                    {"diffuse"  , "cube"},
                    {"canyonTint","canyonTint"}});

   
    
	terrainObject->addComponent(scene->createComponent<Material>(glm::vec3(0.2,0.2,0.2), "terrain_geom_deferred.program",canyon_texture));
	scene->addGameObject(terrainObject);


	LevelLoader levelLoader;
	levelLoader.LoadLevel("scenedata.json", scene);

	LevelBuildingVisual buildingVisual(scene);
	
	float accumTime;
	int lastUpdateTime;
	scene->addCustomUpdate([&](float dt){
		if(Keyboard::key(GLFW_KEY_L))
		{
			if(Keyboard::isKeyToggled(GLFW_KEY_L))
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			}
		}
		Library::TextureLib->Debug_ShowAllTextures();
		//ImGui::ShowTestWindow();
		// accumTime += dt;
		// if((int)accumTime > lastUpdateTime)
		// {
		// 	LOG(GAME, "FPS: " + std::to_string(1.0/dt));
		// 	LOG(GAME, "Active Objects: " + std::to_string(scene->getGameObjects().size()));
			
		// 	lastUpdateTime = (int)accumTime;
		// }
				
	});

	stringmap grassMap {{"diffuse","grassTexture.png"}};
    std::shared_ptr<GameObject> grass  = std::make_shared<GameObject>();
    grass->addComponent(scene->createComponent<Grass>("grass.obj",false,8096));
    grass->addComponent(scene->createComponent<Material>(glm::vec3(1.0,1.0,1.0),"grass.program",grassMap,false));
    scene->addGameObject(grass);


    std::shared_ptr<GameObject> gameState = std::make_shared<GameObject>();
    gameState->addComponent(scene->createComponent<GameState>(PLAYING_STATE));
    scene->addGameObject(gameState);

    DeferredRenderer * renderer = new DeferredRenderer(width, height,
        "SSAO.program", "SSAOBlur.program",
        "shadow_maps.program", "deferred_stencil.program", 
        "deferred_pointL.program", "deferred_dirL.program");
    //renderer->addPostProcessStep(std::make_shared<BasicProgramStep>("postprocess/post_passthrough.program",COMPOSITE_TEXTURE));
    //renderer->addPostProcessStep(std::make_shared<BloomStep>(width, height));
    //renderer->addPostProcessStep(std::make_shared<GUIStep>(width, height));
    //renderer->addPostProcessStep(std::make_shared<HDRStep>("postprocess/bloom/post_HDR_tonemap.program"));

	app->run(scene, renderer);

	delete scene;
	delete renderer;
	
	
	return 0;

}
