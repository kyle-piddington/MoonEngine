// #include "AssimpLoader.h"
// #include "assimp/Importer.hpp"
// #include "assimp/scene.h"
// #include "assimp/postprocess.h"
// #include "Util/Logger.h"
// using namespace MoonEngine;

// bool processMesh(aiMesh * mesh, const AIScene * scene, std::vector<float> & data, std::vector<unsigned> & indices, AssimpModelInfo * outInfo)
// {
	

// }

// bool processNode(aiNode * node, const AIScene * scene, std::vector<float> & data, std::vector<unsigned> & indices, AssimpModelInfo * outInfo)
// {
// 	bool OK = true;
// 	if(node != nullptr)
// 	{
// 		for(uint i = 0; i < node->mNumMeshes; i++)
// 		{
// 			aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
// 			OK &= processMesh(aiMesh, data, indices, aiMesh, outInfo);
// 		}
// 		for(int i = 0; i < node->mNumChildren; i++)
// 		{
// 			OK &= processNode(node->mChildren[i], scene, data, indices, outInfo);
// 		}
// 		return OK;
// 	}
// }
// static bool LoadIntoBuffer(
// 	std::string fileName,
// 	GLBuffer * vertexBuffer,
// 	GLBuffer * indexBuffer,
// 	GLVertexArrayObject * vao,
// 	AssimpModelInfo * outInfo,
// 	unsigned int pFlags,
// 	bool smooth = false)
// {
// 	Assimp::Importer importer;
// 	const aiScene * scene = importer.ReadFile(fileName, pFlags | aiProcess_Triangulate);
// 	bool OK = true;
// 	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
// 	{
// 		LOG(ERROR, "Could not load assimp scene: " + std::string(importer.GetErrorString()));	
// 		OK = false;
// 	}
// 	std::vector<float> data;
// 	std::vector<unsigned> indices;
// 	OK = processNode(scene->mRootNode, scene, data, indices, outInfo)
// 	//
	
// }