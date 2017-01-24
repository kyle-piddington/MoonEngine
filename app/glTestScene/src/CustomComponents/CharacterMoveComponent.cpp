#include "CharacterMoveComponent.h"
using namespace MoonEngine;
#include <iostream>
glm::vec3 CharacterMoveComponent::palette[] = 
	{	GLMUtil::FromHex(0x23F0C7),
		GLMUtil::FromHex(0xEF767A),
		GLMUtil::FromHex(0x7D7ABC),
		GLMUtil::FromHex(0x6457A6),
		GLMUtil::FromHex(0xE3d7FF)
	};
void CharacterMoveComponent::start()
{
	_material = gameObject->getComponent<Material>();
	assert(_material != nullptr);
	_material->setTint(palette[rand()%5]);
	direction = glm::normalize(glm::vec3(rand(),0,rand()));

}

void CharacterMoveComponent::update(float dt)
{
	Transform & t = gameObject->getTransform();
	//Keep in box
	if((t.getPosition().x > 5 && direction.x > 0) || (t.getPosition().x < -5 && direction.x < 0))
	{
		direction.x = -direction.x;
	}
	if((t.getPosition().z > 5 && direction.z > 0) || (t.getPosition().z < -5 && direction.z < 0))
	{
		direction.z = -direction.z;
	}
	gameObject->getTransform().translate(direction * speed * dt, Space::WORLD);
	//Models were oriented incorrectly in blender / export.
	t.lookAt(t.getPosition() - direction);
	

}

void CharacterMoveComponent::onCollisionEnter(Collision collision)
{
	CharacterMoveComponent * other = collision.other->getComponent<CharacterMoveComponent>();
	if(other != nullptr)
	{
		//If velocity moves towards collision point, flip
		glm::vec3 centerPos = (collision.other->getTransform().getPosition()
							   + gameObject->getTransform().getPosition());
		centerPos*=0.5f;
		glm::vec3 normal =  glm::normalize(centerPos - gameObject->getTransform().getPosition());
		if(glm::dot(centerPos - gameObject->getTransform().getPosition(), direction) > 0)
		{
			if(glm::length(normal) > 0)
			{
				direction = glm::reflect(direction, normal);

			}
			else
			{
				direction = -direction;
			}
			speed = rand() % 3 + 1.0;
			_material->setTint(palette[rand()%5]);		

		}
	}
}

std::shared_ptr<Component> CharacterMoveComponent::clone() const
{
	return std::make_shared<CharacterMoveComponent>(*this);
}