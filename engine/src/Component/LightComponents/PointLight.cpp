#include "PointLight.h"

using namespace MoonEngine;

PointLight::PointLight(glm::vec3 position, glm::vec3 color): Light(color){
    _bSphere = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed("sphere.obj", false);
}

std::shared_ptr<Component> PointLight::clone() const
{
    return std::make_shared<PointLight>(*this);
}

void PointLight::setRange(float range)
{
    _attenuation.linear = 4.5f / range;
    _attenuation.exp = 75.0f / (range*range);
}
