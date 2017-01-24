#pragma once
#include "MoonEngine.h"

class ObjectSpawner : public MoonEngine::Component
{

public:
	ObjectSpawner(float spawnTime, std::vector<MoonEngine::GameObject *> prefabs):
	_spawnTime(spawnTime),
	_accumTime(0),
	_prefabList(prefabs)
	{
	}
	void update(float dt);

	virtual std::shared_ptr<MoonEngine::Component> clone() const;

private:
	float _spawnTime;
	float _accumTime;
	std::vector<MoonEngine::GameObject *> _prefabList;



};
