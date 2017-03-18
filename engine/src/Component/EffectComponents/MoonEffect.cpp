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
	direction = glm::vec3(x, y, z);
	player = GetWorld()->getPlayer();
    scale = glm::vec3(0.15);
	gameObject->getTransform().setScale(scale);
}

void MoonEffect::update(float dt)
{
	accumTime += dt;

    if (player != nullptr)
    {
        distance = player->getTransform().getPosition() - gameObject->getTransform().getPosition();
    }

    float x, z;
    glm::vec3 to;
	switch (state)
	{
		case INIT:
			gameObject->getTransform().translate(dt * direction * (1.4f - accumTime));
            scale = glm::vec3(scale.x - 0.02, scale.y, scale.z - 0.02);
            gameObject->getTransform().setScale(scale);
            if (accumTime >= 0.2f)
			{
				state = PAUSE;
			}
			break;
		case PAUSE:
            if (accumTime >= 2.0f)
			{
				state = HOVER;
			}
			break;
        case HOVER:
            to = glm::vec3(0.0, gameObject->getTransform().getPosition().y + 3.0, 0.0);
            gameObject->getTransform().translate(normalize(to) / 10.0f);

            if (accumTime >= 3.0f)
            {
                state = GATHER;
            }

                break;
		case GATHER:
			float scale = length(distance) / 4.0f;

			gameObject->getTransform().translate(normalize(distance) / 2.0f);
			gameObject->getTransform().setScale(std::min(0.15f, scale));
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
