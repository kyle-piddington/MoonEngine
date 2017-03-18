#pragma once

#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "Component/Component.h"
#include "Component/CollisionComponents/BoxCollider.h"
#include "Geometry/Transform.h"
#include "Particle.h"

namespace MoonEngine
{
	class MoonEffect : public Component
	{
	public:

		MoonEffect();

        void start();

        void update(float dt);

        std::shared_ptr<Component> clone() const;

	private:
        float accumTime;
        glm::vec3 direction;
        glm::vec3 distance;

        glm::vec3 scale;
        ParticleState state;
        std::shared_ptr<GameObject> player;
	};
}