#include "Wisp.h"
#include "GameObject/GameObject.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include <iostream>

using namespace MoonEngine;

Wisp::Wisp() :
	accumTime(0)
{

}

void Wisp::start()
{
	player = GetWorld()->getPlayer();
	BoundingBox box = player->getBounds();

}