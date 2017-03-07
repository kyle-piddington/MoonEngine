#pragma once

#include "Component/Component.h"
#include "Component/CollisionComponents/BoxCollider.h"
#include "Geometry/Transform.h"

namespace MoonEngine
{
    class CollectableComponent: public Component
    {
    public:
        CollectableComponent(std::string eventName = "picked_up_star");

		
		void start();

        void update(float dt);

        void onCollisionEnter(Collision col);

        std::shared_ptr<Component> clone() const;
    private:
        std::string _eventName;
        bool _collected;
    };
}
