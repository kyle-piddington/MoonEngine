#pragma once
#include "Component/Component.h"
#include <memory>

namespace MoonEngine
{
    class PointLight : public Component
    {
    public:
        PointLight() {}

        virtual std::shared_ptr<Component> clone() const;

    };
}