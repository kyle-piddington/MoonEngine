#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MoonEngine.h"
#include "LevelEditor/LevelLoader.h"

using namespace MoonEngine;


int main(int argc, char ** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    float windowWidth = 640.0f, windowHeight = 480.0f;
    GLFWwindow * window = glfwCreateWindow(windowWidth, windowHeight, "ShatteredMoon", nullptr, nullptr);
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
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    Logger::SetLogLevel(GAME);
    std::shared_ptr<EngineApp> app = std::make_shared<EngineApp>(window);
    Scene * scene = new Scene();



    //Game Objects
    std::shared_ptr<GameObject> cameraObj = std::make_shared<GameObject>();

    std::shared_ptr<GameObject> playerObj = std::make_shared<GameObject>();
    playerObj->addComponent(scene->createComponent<ThirdPersonCharacterController>(2.1));

    stringmap textures({{"diffuse", "wolf.tga"}});

    playerObj->addComponent(scene->createComponent<StaticMesh>("wolf.obj", false));
    playerObj->addComponent(scene->createComponent<Material>(glm::vec3(0.2, 0.2, 0.2), "geom.program", textures));
    playerObj->addComponent(scene->createComponent<BoxCollider>());

    playerObj->getTransform().setPosition(glm::vec3(0, 0.5, 0));
    playerObj->getTransform().setScale(glm::vec3(0.2, 0.2, 0.2));
    playerObj->addTag(T_Player);

    scene->addGameObject(playerObj);

    //Camera setup
    Camera * cam = scene->createComponent<Camera>(3.1415 / 3, windowWidth / windowHeight, 0.1, 1200);
    cameraObj->addComponent(cam);
    cameraObj->addComponent(scene->createComponent<ThirdPersonOrbitalController>());
    cameraObj->getTransform().translate(glm::vec3(0, 5, 5));
    //cameraObj->getTransform().rotate(glm::vec3(-M_PI/6,0,0));
    scene->addGameObject(cameraObj);


    //Ground
    Transform groundTransform;
    groundTransform.setScale(glm::vec3(5, 1, 5));
    std::shared_ptr<GameObject> groundObject = std::make_shared<GameObject>(groundTransform);
    groundObject->addComponent(scene->createComponent<StaticMesh>("quad.obj", true));
    groundObject->addComponent(scene->createComponent<Material>(glm::vec3(0.2, 0.8, 0.2), "geom.program"));
    scene->addGameObject(groundObject);

    // LevelLoader levelLoader;
    // levelLoader.LoadLevel("scenedata.json", scene);
    stringmap cube_texture({{"diffuse", "cube"}});

    //Upper Platforms
    Transform levelTransform;
    levelTransform.setScale(glm::vec3(1, 0.5, 1));
    levelTransform.setPosition(glm::vec3(-1, 2, 2));
    std::shared_ptr<GameObject> boxObject = std::make_shared<GameObject>(levelTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.9, 0.5, 0.5), "geom.program", cube_texture));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);

    levelTransform.setPosition(glm::vec3(-2, 1.5, 2));
    boxObject = std::make_shared<GameObject>(levelTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.9, 0.5, 0.5), "geom.program", cube_texture));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);


    levelTransform.setPosition(glm::vec3(-1, 2.5, 0.5));
    boxObject = std::make_shared<GameObject>(levelTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.3, 0.5, 0.8), "geom.program", cube_texture));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);

    levelTransform.setPosition(glm::vec3(-1, 3, -1));
    boxObject = std::make_shared<GameObject>(levelTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.9, 0.5, 0.5), "geom.program", cube_texture));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);

    levelTransform.setPosition(glm::vec3(-1, 2.5, -3.5));
    boxObject = std::make_shared<GameObject>(levelTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.2, 0.9, 0.5), "geom.program", cube_texture));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);

    levelTransform.setPosition(glm::vec3(5, 3, 1));
    boxObject = std::make_shared<GameObject>(levelTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.2, 0.9, 0.5), "geom.program", cube_texture));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(boxObject);

	levelTransform.setScale(glm::vec3(0.2, 0.2, 0.2));
	levelTransform.setPosition(glm::vec3(-1, 3, -4));
	std::shared_ptr<GameObject> collectable = std::make_shared<GameObject>(levelTransform);
	collectable->addComponent(scene->createComponent<StaticMesh>("penguin.obj", false));
	collectable->addComponent(scene->createComponent<Material>(glm::vec3(0.2, 0.2, 0.2), "geom.program", textures));
	collectable->addComponent(scene->createComponent<BoxCollider>());
	collectable->addComponent(scene->createComponent<CollectableComponent>());
	collectable->addComponent(scene->createComponent<ShardMovement>());
	scene->addGameObject(collectable);

	levelTransform.setPosition(glm::vec3(-1, 3, -1));
	collectable = std::make_shared<GameObject>(levelTransform);
	collectable->addComponent(scene->createComponent<StaticMesh>("penguin.obj", false));
	collectable->addComponent(scene->createComponent<Material>(glm::vec3(0.2, 0.2, 0.2), "geom.program", textures));
	collectable->addComponent(scene->createComponent<BoxCollider>());
	collectable->addComponent(scene->createComponent<CollectableComponent>());
	collectable->addComponent(scene->createComponent<ShardMovement>());
	scene->addGameObject(collectable);

	levelTransform.setPosition(glm::vec3(-1, 3, -3));
	collectable = std::make_shared<GameObject>(levelTransform);
	collectable->addComponent(scene->createComponent<StaticMesh>("penguin.obj", false));
	collectable->addComponent(scene->createComponent<Material>(glm::vec3(0.2, 0.2, 0.2), "geom.program", textures));
	collectable->addComponent(scene->createComponent<BoxCollider>());
	collectable->addComponent(scene->createComponent<CollectableComponent>());
	collectable->addComponent(scene->createComponent<ShardMovement>());
	scene->addGameObject(collectable);
    //Instance Boxes
    /*std::shared_ptr<GameObject> boxObjects = std::make_shared<GameObject>();
    boxObjects->addComponent(scene->createComponent<InstanceMesh>("cube.obj", "cube.dat", false));
    boxObjects->addComponent(scene->createComponent<Material>(glm::vec3(0.9, 0.8, 0.2), "instance_phong.program", cube_texture));
    scene->addGameObject(boxObjects);*/

    //Boxes
    Transform boxTransform;
    boxTransform.setPosition(glm::vec3(1, 0, 0));
    boxObject = std::make_shared<GameObject>(boxTransform);
    boxObject->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
    boxObject->addComponent(scene->createComponent<Material>(glm::vec3(0.8, 0.8, 0.8), "geom.program", cube_texture));
    boxObject->addComponent(scene->createComponent<BoxCollider>());
    //boxObject->addComponent(scene->createComponent<CollectableComponent>()); Twas a test
    scene->addGameObject(boxObject);




    //Skybox
    Transform skydomeTransform;
    skydomeTransform.setPosition(glm::vec3(0, 0, 0));
    skydomeTransform.setScale(glm::vec3(1000, 1000, 1000));
    std::shared_ptr<GameObject> sphereObject = std::make_shared<GameObject>(boxTransform);
    sphereObject = std::make_shared<GameObject>(skydomeTransform);
    sphereObject->addComponent(scene->createComponent<StaticMesh>("sphere.obj", false));
    stringmap sky_textures({{"skycolor", "skycolor"}});
    sphereObject->addComponent(
            scene->createComponent<Material>(glm::vec3(1.0, 1.0, 1.0), "skydome.program", sky_textures, true));
    scene->addGameObject(sphereObject);


    //Lights
    Transform lightTransform;
    lightTransform.setPosition(glm::vec3(6, 4, 1));
    std::shared_ptr<GameObject> pointLight = make_shared<GameObject>(lightTransform);
    pointLight->addComponent(scene->createComponent<PointLight>(pointLight->getTransform().getPosition(), COLOR_PURPLE, 0.2f, 0.2f));
    pointLight->getComponent<PointLight>()->setRange(10);
    //scene->addGameObject(pointLight);

    lightTransform.setPosition(glm::vec3(-5, 3, 1));
    pointLight = make_shared<GameObject>(lightTransform);
    pointLight->addComponent(scene->createComponent<PointLight>(pointLight->getTransform().getPosition(), COLOR_WHITE, 0.2f, 0.2f));
    pointLight->getComponent<PointLight>()->setRange(10);
    //scene->addGameObject(pointLight);

    lightTransform.setPosition(glm::vec3(4, 3, -5));
    pointLight = make_shared<GameObject>(lightTransform);
    pointLight->addComponent(scene->createComponent<PointLight>(pointLight->getTransform().getPosition(), COLOR_CYAN, 0.2f, 0.2f));
    pointLight->getComponent<PointLight>()->setRange(10);
    //scene->addGameObject(pointLight);


    std::shared_ptr<GameObject> dirLight = make_shared<GameObject>();
    dirLight->addComponent(scene->createComponent<DirLight>(glm::vec3(-1, -1, -1), COLOR_WHITE, 0.1f, 0.5f));
    scene->addGameObject(dirLight);


    //Terrain
    //Preload canyon 32f texture
    EngineApp::GetAssetLibrary().TextureLib->getTexture("grandCanyon",".png",true);
    
    stringmap canyon_texture(
            {{"heightmap", "grandCanyon"}});
  
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
    mapDims.minCoords.y = -5.0f;
    createInfo.dimensions = mapDims;
    std::shared_ptr<GameObject> terrainObject = std::make_shared<GameObject>(Transform());
    terrainObject->addComponent(scene->createComponent<Terrain>(createInfo));
    terrainObject->addComponent(scene->createComponent<Material>(glm::vec3(0.2,0.2,0.2), "terrain_geom_deferred.program",canyon_texture));
    scene->addGameObject(terrainObject);
    
    float accumTime;
    int lastUpdateTime;
    scene->addCustomUpdate([&](float dt) {
        //ImGui::ShowTestWindow();
        // accumTime += dt;
        // if((int)accumTime > lastUpdateTime)
        // {
        // 	LOG(GAME, "FPS: " + std::to_string(1.0/dt));
        // 	LOG(GAME, "Active Objects: " + std::to_string(scene->getGameObjects().size()));

        // 	lastUpdateTime = (int)accumTime;
        // }

    });

	DeferredRenderer * renderer = new DeferredRenderer(width, height, "phong_point_deferred.program", "phong_dir_deferred.program");
    app->run(scene, renderer);

    delete scene;
    delete renderer;

    return 0;

}