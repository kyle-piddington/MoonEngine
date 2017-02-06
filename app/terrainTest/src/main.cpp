#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MoonEngine.h"


using namespace MoonEngine;


int main(int argc, char **argv) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	float windowWidth = 800.0f, windowHeight = 600.0f;
	GLFWwindow * window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", nullptr, nullptr);
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


	MoonEngineCfg cfg;
	cfg.assetPath = "resources";
	
	std::shared_ptr<EngineApp> app = std::make_shared<EngineApp>(window, cfg);
	Scene * scene = new Scene();

	//Game Objects
	std::shared_ptr<GameObject> cameraObj = std::make_shared<GameObject>();



	//Camera setup
	Camera * cam = scene->createComponent<Camera>( 3.1415/3, windowWidth/windowHeight, 0.1, 50);
	cameraObj->addComponent(cam);
	cameraObj->addComponent(scene->createComponent<FirstPersonController>(5));
	scene->addGameObject(cameraObj);



	std::shared_ptr<GameObject> boxObject = std::make_shared<GameObject>(Transform());
	boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
	boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.9, 0.5, 0.5), "phong.program"));
	boxObject->addComponent(scene->createComponent<BoxCollider>());
	//scene->addGameObject(boxObject);

	CDLODQuadtree::CreateInfo createInfo;
	ImplicitHeightmapSource impHeightSource(4096,4096,[](int x, int y){return 65535 * (0.5 + (0.25*cos(10*x/4096.f) + 0.25*sin(10*y/4096.f)));});
	createInfo.source = &impHeightSource;
	createInfo.leafNodeSize = 4;
	createInfo.LODLevelCount = 8;
	MapDimensions mapDims;
	
	mapDims.size = glm::vec3(50,5,50);
	mapDims.minCoords = -mapDims.size/2.0f;
	mapDims.minCoords.y = 0;
	createInfo.dimensions = mapDims;
	std::shared_ptr<GameObject> terrainObject = std::make_shared<GameObject>(Transform());
	terrainObject->addComponent(scene->createComponent<Terrain>(createInfo));
	terrainObject->addComponent(scene->createComponent<Material>(glm::vec3(0.2,0.2,0.2), "terrain.program"));
	scene->addGameObject(terrainObject);
	
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
		//ImGui::ShowTestWindow();
		// accumTime += dt;
		// if((int)accumTime > lastUpdateTime)
		// {
		// 	LOG(GAME, "FPS: " + std::to_string(1.0/dt));
		// 	LOG(GAME, "Active Objects: " + std::to_string(scene->getGameObjects().size()));
			
		// 	lastUpdateTime = (int)accumTime;
		// }
				
	});

	

	ProgramRenderer * renderer = new ProgramRenderer();
	app->run(scene, renderer);

	delete scene;
	delete renderer;
	
	
	return 0;

}