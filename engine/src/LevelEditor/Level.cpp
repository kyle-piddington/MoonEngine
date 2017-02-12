#include "Level.h"

using namespace MoonEngine;

void Level::addLevelMaterial(std::string materialName, MoonEngine::Level::LevelMaterial levelMaterial)
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