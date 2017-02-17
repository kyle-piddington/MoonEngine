#pragma once
#include "GLWrapper/GLTexture.h"
#include "GLUtil/GLTextureConfiguration.h"
#include "Geometry/Terrain/IHeightmapSource.h"
#include "Geometry/Terrain/MapDimensions.h"
#include <memory>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <random>
#include <algorithm>
#include "Util/MathUtil.h"
#include "Util/Logger.h"
namespace MoonEngine
{
	/**
	 * Given a heightmap texture, create a normal map out of it.
	 * And store the texture in a corresponding GLTexture
	 * This call is synchronous.
	 */
	class GLNormalMapCreator
	{
	public:
		GLNormalMapCreator();
		/**
		 * Generaete a normal map at a particular
		 * resolution given a configuration.
		 * The configuration must be a RGBA texture.
		 * The alpha channel of the normal map stores the ambient term
		 * of the heightmap. (Using some basic AO algorithms)
		 */
		std::shared_ptr<GLTexture> GenerateNormalMap(IHeightmapSource * heightmap, const GLTextureConfiguration & newTextureConf, const MapDimensions & mDims);


	};
}
