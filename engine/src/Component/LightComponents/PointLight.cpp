#include "PointLight.h"

using namespace MoonEngine;

PointLight::PointLight(glm::vec3 position, glm::vec3 color): Light(color){
    

    _lightRange.setScale(glm::vec3(32, 32, 32));
    _bSphere = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed("sphere.obj", false);
    _attenuation.constant = 1.0f;
    _attenuation.linear = 0.14;
    _attenuation.exp = 0.07;

}

void PointLight::start()
{
    _lightRange.setParent(&(gameObject->getTransform()));
}

std::shared_ptr<Component> PointLight::clone() const
{
    return std::make_shared<PointLight>(*this);
}

void PointLight::setRange(float range)
{
    _lightRange.setScale(glm::vec3(range, range, range));
    _attenuation.linear = 4.5f / range;
    _attenuation.exp = 75.0f / (range*range);
}

Transform & MoonEngine::PointLight::getLightTransform()
{
    return _lightRange;
    // TODO: insert return statement here
}
