#include <iostream>
#include <IO/TextLoader.h>
#include <thirdparty/rapidjson/prettywriter.h>
#include <fstream>

#include "Level.h"

using namespace MoonEngine;

Level::Level(std::string resourcePath):
    _recPath(resourcePath + "/")
{
}

Level::~Level()
{
    _levelObjects.clear();
}

void Level::addLevelMaterial(std::string materialName, LevelMaterial levelMaterial)
{
    _levelMaterials[materialName] = levelMaterial;
}

void Level::addLevelMaterial(std::string materialName, std::string mesh, Material * material, bool collider)
{
    LevelMaterial levelMaterial = {mesh, material, collider};
    _levelMaterials[materialName] = levelMaterial;
}

Level::LevelMaterial * MoonEngine::Level::getLevelMaterial(std::string materialName)
{
    return &_levelMaterials[materialName];
}

void Level::addLevelObject(LevelObject levelObject) {
    _levelObjects.push_back(levelObject);
}

void Level::removeLevelObject() {
    _levelObjects.pop_back();
}

std::vector<string> Level::getAllLevelMaterials() {
    std::vector<std::string> allLevelMaterials;
    allLevelMaterials.reserve(_levelMaterials.size());
    for (auto levelMaterial : _levelMaterials) {
        allLevelMaterials.push_back(levelMaterial.first);
    }

    return allLevelMaterials;
}

void Level::writeJsonFile(rapidjson::Document & document)
{
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
    document.Accept(writer);

    std::ofstream file("scenedata.json");
    file << buffer.GetString();
}

rapidjson::Value Level::serializeVec3(glm::vec3 obj, rapidjson::Document::AllocatorType& allocator) {
    rapidjson::Value array(rapidjson::kArrayType);
    array.PushBack(rapidjson::Value().SetFloat(obj.x), allocator);
    array.PushBack(rapidjson::Value().SetFloat(obj.y), allocator);
    array.PushBack(rapidjson::Value().SetFloat(obj.z), allocator);

    return array;

}

void Level::serializeLevelObjects()
{
    std::cout << "OUt" << endl;
    std::string levelInfo = TextLoader::LoadFullFile(_recPath + "scenedata.json");

    rapidjson::Document document;
    document.Parse(levelInfo.c_str());

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    rapidjson::Value mapArray(rapidjson::kArrayType);

    for (auto levelObject : _levelObjects) {
        rapidjson::Value levelObjectValue(rapidjson::kObjectType);

        rapidjson::Value material(levelObject.levelMaterial.c_str(), levelObject.levelMaterial.size(), allocator);
        levelObjectValue.AddMember("material", material, allocator);

        Transform * transform = levelObject.transform;
        levelObjectValue.AddMember("position", serializeVec3(transform->getPosition(), allocator), allocator);
        levelObjectValue.AddMember("rotation", serializeVec3(transform->getRotationVec3(), allocator), allocator);
        levelObjectValue.AddMember("scale", serializeVec3(transform->getScale(), allocator), allocator);

        mapArray.PushBack(levelObjectValue, allocator);
    }

    document.RemoveMember("map");
    document.AddMember("map", mapArray, allocator);

    std::cout << "created objects" << endl;

    writeJsonFile(document);
}

std::string Level::getRecPath()
{
    return _recPath;
}
