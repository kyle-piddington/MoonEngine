
#pragma once
#include "AssimpModelInfo.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Util/Logger.h"
#include <algorithm>
namespace MoonEngine
{
	struct AssimpLoader
	{
	/*
	 * Load an assimp mesh into a series of buffers
	 * @param  fileName     fiename of the mesh
	 * @param  vertexBuffer vertex buffer to store vertices in
	 * @param  indexBuffer  index buffer
	 * @param  vao          Vertex array object
	 * @param  outInfo      Info returned by the file
	 * @param  pFlags       Assimp postprocessing flag loading options
	 * @return             true if succesful.
	 */
		static bool LoadIntoBuffer(
			std::string fileName,
			GLBuffer * vertexBuffer,
			GLBuffer * indexBuffer,
			GLVertexArrayObject * vao,
			AssimpModelInfo * outInfo,
			unsigned int pFlags = aiProcessPreset_TargetRealtime_Fast | aiProcess_LimitBoneWeights,
			bool smooth = false);
	};	
}
