#pragma once

#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "Component/Component.h"
#include "Component/CollisionComponents/BoxCollider.h"
#include "Geometry/Transform.h"
#include "Particle.h"

namespace MoonEngine
{
	class ShardEffect : public Component
	{
	public:

		ShardEffect();

        void start();

        void update(float dt);

        std::shared_ptr<Component> clone() const;

	private:
        float accumTime;
        glm::vec3 direction;
        ParticleState state;
        std::shared_ptr<GameObject> player;
	};
}