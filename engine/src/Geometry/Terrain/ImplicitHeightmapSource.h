#include "IHeightmapSource.h"
#include <functional>
#pragma once
namespace MoonEngine
{
	class ImplicitHeightmapSource : public IHeightmapSource
	{
		std::function<int(int,int)> _equation;
		int sizeX;
		int sizeZ;
	public:
		ImplicitHeightmapSource(int x, int z, std::function<int(int,int)> equation):
		sizeX(x),
		sizeZ(z),
		_equation(equation)
		{

		}		
		int getSizeX();

		int getSizeZ();

		unsigned short getHeightAt(int x, int z);

		void getAreaMinMaxY( int x, int z, int sizeX, int sizeZ, unsigned short & minY, unsigned short & maxY );

	};


}