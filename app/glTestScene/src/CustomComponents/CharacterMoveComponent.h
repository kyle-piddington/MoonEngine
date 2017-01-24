#pragma once
#include "MoonEngine.h"

class CharacterMoveComponent : public MoonEngine::Component
{

public:
	CharacterMoveComponent(float velocity):
	_speed(velocity),
	_characterCollideTime(0)
	{
	}

	void start();

	void update(float dt);

	/**
	 * Perform an animation to show that the 
	 * player collided with the character
	 */
	void playerCollided();

	/**
	 * Check if the player is already colliding
	 * @return true if animating
	 */
	bool collidingWithPlayer()
	{
		return _characterCollideTime > 0;
	};

	void onCollisionEnter(MoonEngine::Collision col);
	virtual std::shared_ptr<Component> clone() const;


private:
	void doCharacterCollidedAnimation();
	static glm::vec3 palette[];
	MoonEngine::Material * _material;
	float _speed;
	glm::vec3 _direction;
	glm::vec3 _originalScale;
	float _characterCollideTime;



};
