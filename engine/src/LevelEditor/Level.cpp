#include <iostream>
#include "Level.h"

using namespace MoonEngine;

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

void Level::serializeLevelObjects()
{
    //TODO Save Level Objects to JSON file.
}
