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

    private:
        MeshInfo* _bSphere;
        glm::vec3 _position;
        struct
        {
            float constant;
            float linear;
            float exp;
        } _attenuation;
    };
}