#include <tkDecls.h>
#include <Component/MaterialComponents/Material.h>
#include <Component/MeshComponents/StaticMesh.h>
#include "LevelLoader.h"

#include "thirdparty/rapidjson/document.h"
#include "thirdparty/rapidjson/error/error.h"
#include "thirdparty/rapidjson/error/en.h"
#include "IO/TextLoader.h"
#include "Util/Logger.h"
#include "GLUtil/GLProgramUtilities.h"

using namespace MoonEngine;

bool verifyLevelFile(const rapidjson::Document & document)
{
    bool levelOK = true;
    levelOK &= (document.HasMember("materials"));
    levelOK &= (document["materials"].IsArray());
    if (!levelOK)
    {
        LOG(ERROR, "Level could not be loaded, no materials");
        return levelOK;
    }
    levelOK &= (document.HasMember("map"));
    levelOK &= (document["map"].IsArray());
    if (!levelOK)
    {
        LOG(ERROR, "Level could not be loaded, no materials");
        return levelOK;
    }

    return levelOK;
}

std::string LevelLoader::LoadLevelFromFile(std::string levelName)
{
    std::string fileContents = TextLoader::LoadFullFile(levelName);
    return fileContents;

}

void LevelLoader::LoadLevel(std::string levelName, Scene * scene)
{
    std::string levelInfo = LoadLevelFromFile(levelName);
    //Try and open the file first
    //Parse JSON
    rapidjson::Document document;
    rapidjson::ParseResult pr = document.Parse(levelInfo.c_str());
    if (!pr)
    {
        LOG(ERROR, "ERROR: Could not parse document json. ( " + std::string(rapidjson::GetParseError_En(pr.Code())) +
                   " [" + std::to_string(pr.Offset()) + "] )");
        return;
    }
    verifyLevelFile(document);

    std::string shaderName = document["name"].GetString();
    std::string vertexShaderSource = document["vertex"].GetString();
    std::string fragmentShaderSource = document["fragment"].GetString();

    for (rapidjson::Value::ConstMemberIterator dirs = document["assetDirectories"].MemberBegin(); dirs != document["assetDirectories"].MemberEnd(); ++dirs)
    {
        asset_directories[dirs->name.GetString()] = dirs->value.GetString();
    }

    const rapidjson::Value& materials = document["materials"];
    const rapidjson::Value& map = document["map"];

    Transform transform;
    std::shared_ptr<GameObject> object;

    transform.setScale(glm::vec3(1, 0.5, 1));
    transform.setPosition(glm::vec3(-1, 2, 2));

    stringmap textures({{"Texture", "penguin"}});

    object = std::make_shared<GameObject>(transform);
    object->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
    object->addComponent(scene->createComponent<Material>(glm::vec3(0.9, 0.5, 0.5), "phong.program", textures));
    object->addComponent(scene->createComponent<BoxCollider>());
    scene->addGameObject(object);

    for (auto& material : materials.GetArray())
    {
        printf("%d ", material.GetInt());
    }

    for (auto& object : map.GetArray())
    {
        printf("%d ", material.GetInt());
    }

    for (auto& m : document.GetObject())
        printf("Type of member %s is %s\n",
               m.name.GetString(), kTypeNames[m.value.GetType()]);

}