#include <Component/MaterialComponents/Material.h>
#include <Component/MeshComponents/StaticMesh.h>
#include <iostream>
#include "LevelLoader.h"

#include "IO/TextLoader.h"
#include "Level.h"
#include "Libraries/Library.h"

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

void LevelLoader::LoadLevelMaterials(const rapidjson::Document & document, Scene * scene) {
    const rapidjson::Value & materials = document["materials"];

    /* Load all the level materials */
    for (auto & material : materials.GetArray())
    {
        glm::vec3 tint = glm::vec3(1.0, 1.0, 1.0);

        if (material.HasMember("tint"))
        {
            const rapidjson::Value & tintArray = material["tint"];
            tint = glm::vec3(tintArray[0].GetFloat(), tintArray[1].GetFloat(), tintArray[2].GetFloat());
        }

        string program = material["program"].GetString();

        stringmap textures;
        if (material.HasMember("textures"))
        {
            for (auto & rawTexture : material["textures"].GetObject())
            {
                textures[rawTexture.name.GetString()] = rawTexture.value.GetString();
                LOG(GAME, "Loading " + std::string(rawTexture.name.GetString()) + " as " + std::string(rawTexture.value.GetString()));
            }
        }

        Level::LevelMaterial levelMaterial;
        levelMaterial.mesh = material["mesh"].GetString();
        levelMaterial.material = scene->createComponent<Material>(tint, program, textures);
        levelMaterial.collider = material["collider"].GetBool();

        string name = material["name"].GetString();
        Library::LevelLib->addLevelMaterial(name, levelMaterial);

        LOG(INFO, "Created material object: " + name);
    }
}

void LevelLoader::LoadLevelObjects(const rapidjson::Document & document, Scene * scene) {
    Transform transform;
    std::shared_ptr<GameObject> object;

    const rapidjson::Value & map = document["map"];

    for (auto & mapObject : map.GetArray())
    {
        glm::vec3 position;
        glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);

        const rapidjson::Value & rawPosition = mapObject["position"];
        position = glm::vec3(rawPosition[0].GetFloat(), rawPosition[1].GetFloat(), rawPosition[2].GetFloat());

        if (mapObject.HasMember("rotation"))
        {
            const rapidjson::Value & rawRotation = mapObject["rotation"];
            rotation = glm::vec3(rawRotation[0].GetFloat(), rawRotation[1].GetFloat(), rawRotation[2].GetFloat());
        }
        if (mapObject.HasMember("scale"))
        {
            const rapidjson::Value & rawScale = mapObject["scale"];
            scale = glm::vec3(rawScale[0].GetFloat(), rawScale[1].GetFloat(), rawScale[2].GetFloat());
        }

        transform.setPosition(position);
        transform.setRotation(rotation);
        transform.setScale(scale);

        string rawMaterial = mapObject["material"].GetString();
        Level::LevelMaterial * levelMaterial = Library::LevelLib->getLevelMaterial(rawMaterial);

        object = scene->createGameObject(transform);
        object->addComponent(scene->createComponent<StaticMesh>(levelMaterial->mesh, false));
        object->addTag(T_Dynamic);

        Material * material = scene->cloneComponent<Material>(levelMaterial->material);

        if (mapObject.HasMember("tint"))
        {
            const rapidjson::Value & tintArray = mapObject["tint"];
            material->setTint(glm::vec3(tintArray[0].GetFloat(), tintArray[1].GetFloat(), tintArray[2].GetFloat()));
        }

        object->addComponent(material);

        if (levelMaterial->collider)
        {
            object->addComponent(scene->createComponent<BoxCollider>());
        }

        scene->addGameObject(object);

        Level::LevelObject levelObject = {rawMaterial, &object->getTransform()};
        Library::LevelLib->addLevelObject(levelObject);
    }
}

void LevelLoader::LoadLevel(std::string levelName, Scene * scene)
{
    std::string recPath = Library::LevelLib->getRecPath();
    std::string levelInfo = TextLoader::LoadFullFile(recPath + levelName);

    //Try and open the file first
    //Parse JSON
    rapidjson::Document document;
    rapidjson::ParseResult pr = document.Parse(levelInfo.c_str());
    if (!pr)
    {
        LOG(ERROR, "LEVEL: Could not parse document json. ( " + std::string(rapidjson::GetParseError_En(pr.Code())) +
                   " [" + std::to_string(pr.Offset()) + "] )");
        return;
    }
    verifyLevelFile(document);

    LoadLevelMaterials(document, scene);
    LoadLevelObjects(document, scene);
}

