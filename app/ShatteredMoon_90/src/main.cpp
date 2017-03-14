#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define COLORS_BASIC
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

    float windowWidth = 640, windowHeight = 480;
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

    Logger::SetLogLevel(INFO);
    std::shared_ptr<EngineApp> app = std::make_shared<EngineApp>(window);
    Scene * scene = new Scene();

    AudioService::GetAudio()->loadSound("bgMusic.mp3",true, true);
    AudioService::GetAudio()->playSound("bgMusic.mp3");


    //Game Objects
    Transform playerTransform = Transform();
    playerTransform.setPosition(
            glm::vec3(-32.623940, 20.913505554199219,-101.991371));

    std::shared_ptr<GameObject> cameraObj = std::make_shared<GameObject>(playerTransform);

    playerTransform.setPosition(
            glm::vec3(-52.623940, 12.913505554199219,-101.991371));

    std::shared_ptr<GameObject> playerObj = Library::MeshLib->getGameObjectForModelNamed("Wolf_fbx.fbx","character.program",scene);

    //playerObj.setPosition(playerTransform.getPosition());

    playerObj->addComponent(scene->createComponent<ThirdPersonCharacterController>(4.1));
    // playerObj->addComponent(scene->createComponent<StaticMesh>("wolf.obj", false));
    // playerObj->addComponent(scene->createComponent<Material>(glm::vec3(0.2, 0.2, 0.2), "geom.program", textures));
    playerObj->addComponent(scene->createComponent<BoxCollider>());
    playerObj->addComponent(scene->createComponent<PointLight>(COLOR_CYAN, 3.0f));

    //playerObj->getTransform().setPosition(glm::vec3(0, 0.5, 0));
    playerObj->getTransform().setScale(glm::vec3(1.0, 1.0, 1.0));
    playerObj->addTag(T_Player);

    scene->addGameObject(playerObj);
    
	Transform particleTransform = Transform();
	stringmap particleMap({ { "diffuse", "solid_white" } });
	std::shared_ptr<GameObject> particleObj = std::make_shared<GameObject>(particleTransform);
	particleObj->addComponent(scene->createComponent<StaticMesh>("shard.obj", false));
	particleObj->addComponent(scene->createComponent<Material>(glm::vec3(1, 1, 1), "geom.program", particleMap));
	particleObj->addComponent(scene->createComponent<Particle>());
	particleObj->addComponent(scene->createComponent<PointLight>(glm::vec3(5, 5, 5), 0.5f));

	scene->addPrefab("ShardParticle", particleObj.get());

    //Camera setup
    Camera * cam = scene->createComponent<Camera>(3.1415 / 3, windowWidth / windowHeight, 0.1, 1200);
    cameraObj->addComponent(cam);
    cameraObj->getTransform().translate(glm::vec3(0, 5, 5));
    //cameraObj->getTransform().rotate(glm::vec3(-M_PI/6,0,0));
    scene->addGameObject(cameraObj);


    LevelLoader levelLoader;
    levelLoader.LoadLevel("scenedata.json", scene);
    stringmap cube_texture({{"diffuse", "cube"}});



    std::shared_ptr<GameObject> dirLight = make_shared<GameObject>();
    dirLight->addComponent(scene->createComponent<DirLight>(glm::vec3(-1, -1, -1), COLOR_WHITE, 0.1f, 0.5f));
    scene->addGameObject(dirLight);

    //Terrain
    //Preload canyon 32f texture
    EngineApp::GetAssetLibrary().TextureLib->createImage("grandCanyon",".png",true);

    stringmap canyon_texture(
            {{"heightmap", "grandCanyon"},
                    {"diffuse"  , "cube"},
                    {"canyonTint","canyonTint"}});

    CDLODQuadtree::CreateInfo createInfo;
    //ImplicitHeightmapSource heightSource(256,256,[](int, int){return 0;});
    TextureHeightmapSource texSource("resources/textures","grandCanyon",".png");
    createInfo.source = &texSource;
    createInfo.leafNodeSize = 2;
    createInfo.LODLevelCount = 5;
    MapDimensions mapDims;

    mapDims.size = glm::vec3(1000,200,1000);
    mapDims.minCoords = glm::vec3(0,0,0);
    mapDims.minCoords = -mapDims.size/2.0f;
    mapDims.minCoords.y = 0;
    createInfo.dimensions = mapDims;
    std::shared_ptr<GameObject> terrainObject = std::make_shared<GameObject>(Transform());
    terrainObject->addComponent(scene->createComponent<Terrain>(createInfo));
    terrainObject->addComponent(scene->createComponent<Material>(glm::vec3(0.2,0.2,0.2), "terrain_geom_deferred.program",canyon_texture));
    scene->addGameObject(terrainObject);

    std::shared_ptr<GameObject> guiObject = std::make_shared<GameObject>();
    guiObject->addComponent(scene->createComponent<GUI>(width, height));
    scene->addGameObject(guiObject);


    Transform tran;
    tran.setPosition(glm::vec3(0.0, 150.0, 0.0));
    tran.setScale(glm::vec3(5, 5, 5));

    stringmap sun = {{"billboard", "sun.tga"}};

    std::shared_ptr<GameObject> sunBillboard = std::make_shared<GameObject>(tran);
    sunBillboard->addComponent(scene->createComponent<StaticMesh>("quad", false));
    sunBillboard->addComponent(scene->createComponent<Material>(glm::vec3(1.0, 1.0, 1.0), "billboard.program", sun, true));
    scene->addGameObject(sunBillboard);

    //Grass
    stringmap grassMap {{"diffuse","grassTexture.png"}};
    std::shared_ptr<GameObject> grass  = std::make_shared<GameObject>(playerTransform);
    grass->addComponent(scene->createComponent<Grass>("grass.obj",false,8096*2));
    grass->addComponent(scene->createComponent<Material>(glm::vec3(1.0,1.0,1.0),"grass.program",grassMap,true));
    scene->addGameObject(grass);
    
    
    std::shared_ptr<GameObject> gameState = std::make_shared<GameObject>();
    gameState->addComponent(scene->createComponent<GameState>());
    scene->addGameObject(gameState);


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

    DeferredRenderer * renderer = new DeferredRenderer(width, height, 
        "shadow_maps.program", "deferred_stencil.program", "deferred_pointL.program", "deferred_dirL.program");
    renderer->addPostProcessStep(std::make_shared<BasicProgramStep>("postprocess/post_passthrough.program",COMPOSITE_TEXTURE));
    renderer->addPostProcessStep(std::make_shared<BloomStep>(width, height));
    renderer->addPostProcessStep(std::make_shared<SkyStep>(width, height));
    renderer->addPostProcessStep(std::make_shared<GUIStep>(width, height));
    renderer->addPostProcessStep(std::make_shared<HDRStep>("postprocess/bloom/post_HDR_tonemap.program"));

    app->run(scene, renderer);

    //delete scene;
    //delete renderer;

    AudioService::GetAudio()->shutdown();

    return 0;

}
