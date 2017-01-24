#pragma once
#include "MoonEngine.h"

class CharacterMoveComponent : public MoonEngine::Component
{

public:
	CharacterMoveComponent(float velocity):
	speed(velocity)
	{
	}

	void start();

	void update(float dt);

	void onCollisionEnter(MoonEngine::Collision col);
	virtual std::shared_ptr<Component> clone() const;

private:
	static glm::vec3 palette[];
	MoonEngine::Material * _material;
	float speed;
	glm::vec3 direction;



};
