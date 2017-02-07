#pragma once
#include <string>
#include "IHeightmapSource.h"
namespace MoonEngine
{
	class TextureHeightmapSource : public IHeightmapSource
	{
	public:
		/**
		 * Load a texture for use with a heightmap.
		 */
		TextureHeightmapSource(std::string path, std::string texture, std::string extension = ".png");
		/**
		 * Delete the data buffer
		 */
		~TextureHeightmapSource();
		/**
		 * get a width;
		 * @return width of the texture
		 */
		int getSizeX();

		/**
		 * Get a height
		 * @return height of the texture
		 */
		int getSizeZ();

		/**
		 * Get the height at a pixel
		 * @param  x x pixel
		 * @param  Z z pixel
		 * @return   get the height at some pixel
		 */
		unsigned short getHeightAt(int x, int z);

	private:
		int _texWidth;
		int _texHeight;
		unsigned short * data;
	};
}