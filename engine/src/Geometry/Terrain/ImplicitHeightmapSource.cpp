#include "ImplicitHeightmapSource.h"
#include <algorithm>
using namespace MoonEngine;

int ImplicitHeightmapSource::getSizeX()
{
	return sizeX;
}

int ImplicitHeightmapSource::getSizeZ()
{
	return sizeZ;
}

unsigned short ImplicitHeightmapSource::getHeightAt(int x, int z)
{
	return _equation(x,z);
}

