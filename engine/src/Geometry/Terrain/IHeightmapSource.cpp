#include "IHeightmapSource.h"
#include <algorithm>	
using namespace MoonEngine;

void IHeightmapSource::getAreaMinMaxY(int x, int z, int sizeX, int sizeZ, unsigned short & minY, unsigned short & maxY)
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