#include "IHeightmapSource.h"
#include <algorithm>	
#include <iostream>
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
	int ix = (int)(x);
	int iz = (int)(z);

	int lx = std::min(sx - 1,std::max(0,ix));
	int ly = std::min(sz - 1,std::max(0,iz));
	int ux = std::min(sx - 1, std::max(0,ix + 1));
	int uy = std::min(sz - 1, std::max(0,iz + 1));
	unsigned short sampleBL = getHeightAt(lx,ly);
	unsigned short sampleBR = getHeightAt(ux,ly);
	unsigned short sampleUL = getHeightAt(lx,uy);
	unsigned short sampleUR = getHeightAt(ux,uy);
	float u = (ux - x) /(ux - lx);
	float v = (uy - z) / (uy - ly);
	float xy1 =  u  * sampleBL +  (1 - u) * sampleBR;
	float xy2 =  u * sampleUL + (1 - u) * sampleUR;
	float yInterp = v  * xy1 + (1 - v) * xy2;

	//std::cout << u << " , " << v << std::endl;
	return yInterp;
}