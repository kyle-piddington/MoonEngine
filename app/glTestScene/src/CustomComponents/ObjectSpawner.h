#pragma once
#include "MoonEngine.h"

class ObjectSpawner : public MoonEngine::Component
{

public:
	ObjectSpawner(float spawnTime, std::vector<MoonEngine::GameObject *> prefabs, int maxSpawn = 100):
	_spawnTime(spawnTime),
	_accumTime(0),
	_prefabList(prefabs),
	_numSpawned(0),
	_maxSpawned(maxSpawn)
	{
	}
	void update(float dt);

	virtual std::shared_ptr<MoonEngine::Component> clone() const;

private:
	float _spawnTime;
	float _accumTime;
	int _numSpawned;
	int _maxSpawned;
	std::vector<MoonEngine::GameObject *> _prefabList;



};
