#include "ObjectSpawner.h"
using namespace MoonEngine;
#include <iostream>


void ObjectSpawner::update(float dt)
{
	_accumTime += dt;
	if(_accumTime > _spawnTime && _numSpawned < _maxSpawned)
	{
		Instantiate(_prefabList[rand()%_prefabList.size()]);
		_accumTime = 0;
	}

}



std::shared_ptr<Component> ObjectSpawner::clone() const
{
	return std::make_shared<ObjectSpawner>(*this);
}