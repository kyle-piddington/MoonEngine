#include "MoonEffect.h"

using namespace MoonEngine;

MoonEffect::MoonEffect() :
	accumTime(0),
	state(INIT)
{

}

void MoonEffect::start()
{
	float x = (float)rand() / RAND_MAX * 6.28f - 3.14;
    float y = (float)rand() / RAND_MAX * 6.28f - 3.14;
	float z = (float)rand() / RAND_MAX * 6.28f - 3.14;
	gatherTime = (float)rand()/RAND_MAX + 1.4;
	direction = glm::vec3(x, y, z);
	player = GetWorld()->getPlayer();
    scale = glm::vec3(0.08);
	gameObject->getTransform().setScale(scale);
}

void MoonEffect::update(float dt)
{
	accumTime += dt;

    if (player != nullptr)
    {
        distance = player->getTransform().getPosition() - gameObject->getTransform().getPosition();
    }

    gameObject->getTransform().rotate(direction *dt);
    float moveSpeed = -(log(accumTime) - 1) / 3.0f;

	switch (state)
	{
		case INIT:
			gameObject->getTransform().translate(dt * moveSpeed * direction);
            if (accumTime >= gatherTime)
			{
				state = GATHER;
			}
			break;
		
     
		case GATHER:
			float scale = length(distance) / 16.0f;
			gameObject->getTransform().translate(normalize(distance));
			gameObject->getTransform().setScale(std::min(0.08f, scale));
            gameObject->getTransform().rotate(direction *dt);

            if (scale <= 0.1f)
			{
				Delete(gameObject);
			}
			break;
	}
}

std::shared_ptr<Component> MoonEffect::clone() const
{
	return std::make_shared<MoonEffect>(*this);
}
