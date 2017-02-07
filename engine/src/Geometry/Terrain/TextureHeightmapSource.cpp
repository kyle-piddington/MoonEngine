#include "TextureHeightmapSource.h"
#include "Loaders/TextureLoader.h"
#include <cassert>
#include "thirdparty/stb_image.h"
using namespace MoonEngine;

TextureHeightmapSource::TextureHeightmapSource(std::string path, std::string texture, std::string extension)
{
	int comps = TextureLoader::LoadTextureToBuffer16f(&data, path + "/" + texture + extension, &_texWidth, &_texHeight);
	assert(comps == 1);
}

TextureHeightmapSource::~TextureHeightmapSource()
{
	stbi_image_free(data);
}

int TextureHeightmapSource::getSizeX()
{
	return _texWidth;
}

int TextureHeightmapSource::getSizeZ()
{
	return _texHeight;
}

unsigned short TextureHeightmapSource::getHeightAt(int x, int z)
{
	assert(x < _texWidth);
	assert(z < _texHeight);
	return (data[z * _texWidth + x]);
}