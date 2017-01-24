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
	_direction = glm::normalize(glm::vec3(rand(),0,rand()));
	_originalScale = gameObject->getTransform().getScale();

}

void CharacterMoveComponent::update(float dt)
{
	if(_characterCollideTime > 0)
	{
		doCharacterCollidedAnimation();
		_characterCollideTime -= dt;
	}
	Transform & t = gameObject->getTransform();
	//Keep in box
	if((t.getPosition().x > 5 && _direction.x > 0) || (t.getPosition().x < -5 && _direction.x < 0))
	{
		_direction.x = -_direction.x;
	}
	if((t.getPosition().z > 5 && _direction.z > 0) || (t.getPosition().z < -5 && _direction.z < 0))
	{
		_direction.z = -_direction.z;
	}
	gameObject->getTransform().translate(_direction * _speed * dt, Space::WORLD);
	//Models were oriented incorrectly in blender / export.
	t.lookAt(t.getPosition() - _direction);
	

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
		if(glm::dot(centerPos - gameObject->getTransform().getPosition(), _direction) > 0)
		{
			if(glm::length(normal) > 0)
			{
				_direction = glm::reflect(_direction, normal);

			}
			else
			{
				_direction = -_direction;
			}
			_direction.y = 0;
			_speed = rand() % 3 + 1.0;
			if(!collidingWithPlayer())
			{
				_material->setTint(palette[rand()%5]);						
			}

		}
	}
}

std::shared_ptr<Component> CharacterMoveComponent::clone() const
{
	return std::make_shared<CharacterMoveComponent>(*this);
}

void CharacterMoveComponent::playerCollided()
{
	_characterCollideTime = 1.5;
	_direction = -_direction;
	_material->setTint(glm::vec3(1,1,1));
}

void CharacterMoveComponent::doCharacterCollidedAnimation()
{
	Transform & t = gameObject->getTransform();
	t.setScale(glm::vec3(_originalScale.x, _originalScale.y + (float)0.15*(std::abs(sin(_characterCollideTime * 5) * _characterCollideTime)), _originalScale.z));
}