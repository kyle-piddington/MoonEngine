#include "IHeightmapSource.h"
#include <algorithm>	
using namespace MoonEngine;

void IHeightmapSource::getAreaMinMaxY(int x, int z, int sizeX, int sizeZ, unsigned short & minY, unsigned short & maxY)
{
	minY = maxY = getHeightAt(x,z);
	for(int i = x; i < x + sizeX; i++)
	{
		for (int j = z; j <  z + sizeZ; j++)
		{
			minY = std::min(minY, getHeightAt(i,j));
			maxY = std::max(maxY, getHeightAt(i,j));
		}
	}
}

//Perform bilinear interpolation
float IHeightmapSource::getHeightAtFloat(float x, float z)
{
	int sx = getSizeX();
	int sz = getSizeZ();
	//Samples
	int lx = std::min(sx - 1,std::max(0,(int)(x)));
	int ly = std::min(sz - 1,std::max(0,(int)(z)));
	int ux = std::min(sx - 1, std::max(0,(int)(x + 1)));
	int uy = std::min(sz - 1, std::max(0,(int)(z + 1)));
	unsigned short sampleBL = getHeightAt(lx,ly);
	unsigned short sampleBR = getHeightAt(ux,ly);
	unsigned short sampleUL = getHeightAt(lx,uy);
	unsigned short sampleUR = getHeightAt(ux,uy);
	float xy1 = (ux - x) /(ux - lx) * sampleBL +  (x-lx)/(ux - lx) * sampleBR;
	float xy2 = (ux - x ) /(ux - lx) * sampleUL + (x-lx)/(ux - lx) * sampleUR;
	float yInterp = (uy - z) / (uy - ly)  * xy1 + (z - ly) / (uy - ly) * xy2;
	return yInterp;
}