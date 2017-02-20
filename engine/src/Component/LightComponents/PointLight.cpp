#include "PointLight.h"

using namespace MoonEngine;

PointLight::PointLight(glm::vec3 position, glm::vec3 color, float range, float ambient)
    : Light(color, ambient, 0.8f),  _lightRange(), _position(position)
{
    _attenuation.constant = 1.0f;
    setRange(range);
    _bSphere = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed("sphere.obj", false);

}

PointLight::PointLight(glm::vec3 position, glm::vec3 color, float range) : 
    Light(color, 0.2f, 0.8f), _lightRange(), _position(position)
{
    _attenuation.constant = 1.0f;
    setRange(range);
    _bSphere = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed("sphere.obj", false);
}

PointLight::PointLight(glm::vec3 color, float range) :
Light(color, 0.2f, 0.8f), _lightRange(), _position(glm::vec3(0,0,0))
{
    _attenuation.constant = 1.0f;
    setRange(range);
    _bSphere = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed("sphere.obj", false);
}

PointLight::PointLight(glm::vec3 color) :
    Light(color, 0.2f, 0.8f), _lightRange(), _position(glm::vec3(0,0,0))
{
    _attenuation.constant = 1.0f;
    setRange(32);
    _bSphere = EngineApp::GetAssetLibrary().MeshLib->getInfoForMeshNamed("sphere.obj", false);
}


void PointLight::update(float dt) {
    _lightRange.setPosition(gameObject->getTransform().getPosition());
    _lightRange.setRotation(gameObject->getTransform().getRotation());
}
std::shared_ptr<Component> PointLight::clone() const
{
    return std::make_shared<PointLight>(*this);
}

void PointLight::setRange(float range)
{
    if (range <= 10) {
        _attenuation.linear = 0.7f;
        _attenuation.exp = 1.8f;
    }
    else if (range <= 16) {
        _attenuation.linear = 0.35f;
        _attenuation.exp = 0.20f;
    }
    else if (range <= 26) {
        _attenuation.linear = 0.22f;
        _attenuation.exp = 0.20f;
    }
    else if (range <= 41) {
        _attenuation.linear = 0.14f;
        _attenuation.exp = 0.07f;
    }
    else if (range <= 57) {
        _attenuation.linear = 0.09f;
        _attenuation.exp = 0.032f;
    }
    else {
        _attenuation.linear = 0.07f;
        _attenuation.exp = 0.017f;
    }
}

Transform & MoonEngine::PointLight::getLightTransform()
{
    float maxChannel = fmax(fmax(this->_color.x, this->_color.y), this->_color.z);

    float range = (-_attenuation.linear + sqrtf(_attenuation.linear * _attenuation.linear - 4 * _attenuation.exp 
        * (_attenuation.constant - maxChannel * 256.0/5.0)))  / (2 * _attenuation.exp);
    _lightRange.setScale(range);
    return _lightRange;
    // TODO: insert return statement here
}
