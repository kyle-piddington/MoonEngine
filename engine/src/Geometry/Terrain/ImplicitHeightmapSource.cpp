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

void ImplicitHeightmapSource::getAreaMinMaxY( int x, int z, int sizeX, int sizeZ, unsigned short & minY, unsigned short & maxY )
{
	minY = maxY = getHeightAt(x,z);
	for(int i = x; i < sizeX; i++)
	{
		for (int j = z; j < sizeZ; j++)
		{
			minY = std::min(minY, getHeightAt(i,j));
			maxY = std::max(maxY, getHeightAt(i,j));
		}
	}
}
