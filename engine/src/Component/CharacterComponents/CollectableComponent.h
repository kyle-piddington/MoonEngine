#pragma once

#include "Component/Component.h"
#include "Component/CollisionComponents/BoxCollider.h"
#include "Geometry/Transform.h"

namespace MoonEngine
{
    class CollectableComponent: public Component
    {
    public:
        CollectableComponent();

        void start();

        void update(float dt);

        void onCollisionEnter(Collision col);

        std::shared_ptr<Component> clone() const;
    };
}
