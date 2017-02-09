#pragma once
#include "Light.h"
#include "Loaders/MeshInfo.h"
#include <memory>
#include "EngineApp/EngineApp.h"

namespace MoonEngine
{
    class PointLight : public Light
    {
    public:
        PointLight(glm::vec3 position, glm::vec3 color);

        virtual std::shared_ptr<Component> clone() const;

        void setRange(float distance);

        glm::vec3 getPosition() { return _position; }
        MeshInfo* getSphere() { return _bSphere; }
        glm::vec3 getAttenuation() {
            return glm::vec3(_attenuation.constant, _attenuation.linear, _attenuation.exp);
        }

    private:
        MeshInfo* _bSphere;
        glm::vec3 _position;
        struct Attenuation
        {
            float constant;
            float linear;
            float exp;
        } _attenuation;
    };
}