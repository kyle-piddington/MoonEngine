#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MoonEngine.h"
#include "CustomComponents/CharacterMoveComponent.h"
#include "CustomComponents/ObjectSpawner.h"
#include "CustomComponents/PlayerInteractionComponent.h"


using namespace MoonEngine;


int main(int argc, char **argv) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow * window = glfwCreateWindow(800,600, "LearnOpenGL", nullptr, nullptr);
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

	Logger::SetLogLevel(GAME);


	MoonEngineCfg cfg;
	cfg.assetPath = "resources";
	
	std::shared_ptr<EngineApp> app = std::make_shared<EngineApp>(window, cfg);
	Scene * scene = new Scene();

	//Game Objects
	std::shared_ptr<GameObject> cameraObj = std::make_shared<GameObject>();

	
	//Camera setup
	Camera * cam = scene->createComponent<Camera>( M_PI/3, 800.0/600.0, 0.1, 50);
	FirstPersonController * ctrl = scene->createComponent<FirstPersonController>(5);
	cameraObj->addComponent(ctrl);
	cameraObj->addComponent(cam);
	PlayerInteractionComponent * playerInteractionComp = scene->createComponent<PlayerInteractionComponent>();
	cameraObj->addComponent(playerInteractionComp);
	
	cameraObj->addComponent(scene->createComponent<BoxCollider>(glm::vec3(-0.25), glm::vec3(0.25)));
	cameraObj->getTransform().translate(glm::vec3(0,1,-5));
	scene->addGameObject(cameraObj);

	
	//Monkey prefab setup
	Transform monkeyTransform;
	monkeyTransform.setScale(glm::vec3(0.5,0.5,0.5));
	monkeyTransform.setPosition(glm::vec3(0,0.5,0));
	std::shared_ptr<GameObject> monkeyObj = std::make_shared<GameObject>(monkeyTransform);
	StaticMesh * monkeyMesh = scene->createComponent<StaticMesh>("suzanne.obj", true);
	Material * monkeyMat = scene->createComponent<Material>(glm::vec3(0.6,0.5,0.5),"phong.program");
	CharacterMoveComponent * monkeyMove = scene->createComponent<CharacterMoveComponent>(rand()%30 / 10.f);
	BoxCollider * monkeyCollider = scene->createComponent<BoxCollider>();
	monkeyObj->addComponent(monkeyCollider);
	monkeyObj->addComponent(monkeyMesh);
	monkeyObj->addComponent(monkeyMat);
	monkeyObj->addComponent(monkeyMove);
		
	//Dog prefab setup
	Transform dogTransform;
	dogTransform.setScale(glm::vec3(0.2,0.2,0.2));
	dogTransform.setPosition(glm::vec3(0,0.5,0));
	std::shared_ptr<GameObject> dogObj = std::make_shared<GameObject>(dogTransform);
	StaticMesh * dogMesh = scene->createComponent<StaticMesh>("dog.obj", true);
	Material * dogMaterial = scene->createComponent<Material>(glm::vec3(0.6,0.5,0.5),"phong.program");
	CharacterMoveComponent * dogMove = scene->createComponent<CharacterMoveComponent>(rand()%30 / 10.f);
	BoxCollider * dogCollider = scene->createComponent<BoxCollider>();
	dogObj->addComponent(dogCollider);
	dogObj->addComponent(dogMesh);
	dogObj->addComponent(dogMaterial);
	dogObj->addComponent(dogMove);
	
	//Spawner prefab setup
	std::vector<GameObject *> prefabPtrs;
	prefabPtrs.push_back(dogObj.get());
	prefabPtrs.push_back(monkeyObj.get());

	std::shared_ptr<GameObject> spawnerObj = std::make_shared<GameObject>(Transform());
	spawnerObj->addComponent(scene->createComponent<ObjectSpawner>(3.0f,prefabPtrs));
	scene->addGameObject(spawnerObj);

	//Ground
	Transform groundTransform;
	groundTransform.setScale(glm::vec3(5,1,5));
	std::shared_ptr<GameObject> groundObject = std::make_shared<GameObject>(groundTransform);
	groundObject->addComponent(scene->createComponent<StaticMesh>("quad.obj",true));
	groundObject->addComponent(scene->createComponent<Material>(glm::vec3(0.2,0.8,0.2), "default.program"));
	
	scene->addGameObject(groundObject);

	//Add FPS, Game Object, and winning count
	float accumTime = 0;
	int lastUpdateTime = 0;
	scene->addCustomUpdate([&](float dt){
		accumTime += dt;
		if((int)accumTime > lastUpdateTime)
		{
			LOG(GAME, "FPS: " + std::to_string(1.0/dt));
			LOG(GAME, "Active Objects: " + std::to_string(scene->getGameObjects().size()));
			LOG(GAME, "Touched Objects: " + std::to_string(playerInteractionComp->getNumCollisions()));
			lastUpdateTime = (int)accumTime;
		}
				
	});

	

	ProgramRenderer * renderer = new ProgramRenderer();
	app->run(scene, renderer);

	delete scene;
	delete renderer;
	
	
	return 0;

}