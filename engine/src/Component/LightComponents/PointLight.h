#pragma once
#include "Light.h"
#include "Loaders/BasicMeshInfo.h"
#include <memory>
#include "EngineApp/EngineApp.h"

namespace MoonEngine
{
    class PointLight : public Light
    {
    public:
        PointLight(glm::vec3 position, glm::vec3 color, float range, float ambient);
        PointLight(glm::vec3 position, glm::vec3 color, float range);
        PointLight(glm::vec3 color, float range);
        PointLight(glm::vec3 color);
        

        virtual std::shared_ptr<Component> clone() const;

        void update(float dt);

        void setAttenuation(float linear, float exp);
        void setRange(float distance);

        const glm::vec3& getPosition() { return _position; }
        BasicMeshInfo* getSphere() { return _bSphere; }
        const glm::vec3& getAttenuation() {
            _atten =glm::vec3(_attenuation.constant, _attenuation.linear, _attenuation.exp);
            return _atten;
        }

        Transform& getLightTransform();
    private:
        BasicMeshInfo* _bSphere;
        glm::vec3 _position;
        glm::vec3 _atten;
        Transform _lightRange;
        struct Attenuation
        {
            float constant;
            float linear;
            float exp;
        } _attenuation;
    };
}