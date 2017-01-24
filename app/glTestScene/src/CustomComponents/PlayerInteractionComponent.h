#pragma once
#include "MoonEngine.h"

class PlayerInteractionComponent : public MoonEngine::Component
{

public:
	PlayerInteractionComponent():
	_numCollisions(0){}
	void onCollisionEnter(MoonEngine::Collision col);

	int getNumCollisions()
	{
		return _numCollisions;
	}
	virtual std::shared_ptr<Component> clone() const;

private:
	int _numCollisions;
};
