#include <Component/MaterialComponents/Material.h>
#include <Component/MeshComponents/StaticMesh.h>
#include <iostream>
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

    const rapidjson::Value & materials = document["materials"];
    const rapidjson::Value & map = document["map"];

    /* Transform transform;
     std::shared_ptr<GameObject> object;

     transform.setScale(glm::vec3(1, 0.5, 1));
     transform.setPosition(glm::vec3(-1, 2, 2));

     stringmap textures({{"Texture", "penguin"}});

     object = std::make_shared<GameObject>(transform);
     object->addComponent(scene->createComponent<StaticMesh>("cube.obj", false));
     object->addComponent(scene->createComponent<Material>(glm::vec3(0.9, 0.5, 0.5), "phong.program", textures));
     object->addComponent(scene->createComponent<BoxCollider>());
     scene->addGameObject(object);*/

    for (auto & material : materials.GetArray())
    {
        LevelMaterial levelMaterial;
        levelMaterial.mesh = material["mesh"].GetString();
        std::cout << "Mesh: " << levelMaterial.mesh << endl;

        glm::vec3 tint = glm::vec3(1.0, 1.0, 1.0);

        if (material.HasMember("tint"))
        {
            const rapidjson::Value & tintArray = material["tint"];
            tint = glm::vec3(tintArray[0].GetFloat(), tintArray[1].GetFloat(), tintArray[2].GetFloat());
            std::cout << "Tint: " << tint.x << " " << tint.y << " " << tint.z << endl;
        }

        string program = material["program"].GetString();
        std::cout << "Program: " << program << endl;

        stringmap textures;
        for (auto & rawTexture : material["textures"].GetObject())
        {
            std::cout << "Texture: " << rawTexture.name.GetString() << " " << rawTexture.value.GetString() << endl;
            textures[rawTexture.name.GetString()] = rawTexture.value.GetString();
        }

        levelMaterial.material = scene->createComponent<Material>(tint, program, textures);

        string name = material["name"].GetString();
        std::cout << "Created material object: " << name << endl;

        _levelMaterials[name] = levelMaterial;
    }

    Transform transform;
    std::shared_ptr<GameObject> object;

    for (auto & mapObject : map.GetArray())
    {
        glm::vec3 position;
        glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);

        const rapidjson::Value & rawPosition = mapObject["position"];
        position = glm::vec3(rawPosition[0].GetFloat(), rawPosition[1].GetFloat(), rawPosition[2].GetFloat());
        std::cout << "Position: " << position.x << " " << position.y << " " << position.z << endl;

        if (mapObject.HasMember("rotation"))
        {
            const rapidjson::Value & rawRotation = mapObject["rotation"];
            rotation = glm::vec3(rawRotation[0].GetFloat(), rawRotation[1].GetFloat(), rawRotation[2].GetFloat());
            std::cout << "Rotation: " << rotation.x << " " << rotation.y << " " << rotation.z << endl;
        }
        if (mapObject.HasMember("scale"))
        {
            const rapidjson::Value & rawScale = mapObject["scale"];
            scale = glm::vec3(rawScale[0].GetFloat(), rawScale[1].GetFloat(), rawScale[2].GetFloat());
            std::cout << "Scale: " << scale.x << " " << scale.y << " " << scale.z << endl;
        }

        transform.setPosition(position);
        transform.setRotation(rotation);
        transform.setScale(scale);

        string rawMaterial = mapObject["material"].GetString();
        LevelMaterial levelMaterial = _levelMaterials[rawMaterial];

        object = std::make_shared<GameObject>(transform);
        object->addComponent(scene->createComponent<StaticMesh>(levelMaterial.mesh, false));
        object->addComponent(levelMaterial.material->clone().get());

        if (levelMaterial.collider)
        {
            object->addComponent(scene->createComponent<BoxCollider>());
        }

        scene->addGameObject(object);
    }
}