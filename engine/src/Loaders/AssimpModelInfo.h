#pragma once
#include "GLWrapper/GLVertexArrayObject.h"
#include "Collision/BoundingBox.h"
#include "BasicMeshInfo.h"
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
//AssimpInfo contains all
//data imported by assimp, metadata
//about the mesh, and a meshInfo 
//for binding later.
namespace MoonEngine
{

	struct AssimpBoneInfo
	{
		AssimpBoneInfo():
		parentBoneIndex(-1),
		boneName(""),
		offsetMatrix(1.0),
		childBones(){}
		int parentBoneIndex;
		std::string boneName;
		glm::mat4 offsetMatrix;
		std::vector<int>childBones;
	};

	struct AssimpPositionKeyFrame
	{
		AssimpPositionKeyFrame(glm::vec3 position, float time):
			position(position),
			mTime(time){}
		glm::vec3 position;
		float mTime;
	};

	struct AssimpRotationKeyFrame
	{
		AssimpRotationKeyFrame(glm::quat rotation, float time):
			rotation(rotation),
			mTime(time){}
		glm::quat rotation;
		float mTime;
	};

	struct AssimpScaleKeyFrame
	{
		AssimpScaleKeyFrame(glm::vec3 scale, float time):
			scale(scale),
			mTime(time){}
		glm::vec3 scale;
		float mTime;
	};

	struct AssimpBoneAnimInfo
	{
		std::string boneName;
		std::vector<AssimpPositionKeyFrame> translationKeys;
		std::vector<AssimpRotationKeyFrame> rotationKeys;
		std::vector<AssimpScaleKeyFrame> scaleKeys;
	};

	struct AssimpAnimationInfo
	{
		std::string name;
		double duration;
		double ticksPerSecond;
		std::vector<AssimpBoneAnimInfo> boneAnimData;
	};

	struct AssimpMeshInfo
	{
		std::unordered_map<std::string, std::string> textures;
		BasicMeshInfo meshInfo;
		BoundingBox box;
		glm::mat4 M;
		//Mat4 offsets for binding matricies.
		std::vector<glm::mat4> boneOffsets;


		void bind() const
		{
			meshInfo.bind();
		}
	};

	/**
	 * Contains all information about a model loaded by Assimp
	 * (+ Textures, full mesh info, bone and armature data (Eventually!))
	 */
	struct AssimpModelInfo
	{

        //Bounding min and max vertices
		BoundingBox boundingBox;

		bool hasNormals() const;

		bool hasTangentBitangent() const;

		bool hasTextureCoordinates() const;

		bool hasBones() const;

		const std::vector<AssimpMeshInfo> & getMeshInfo() const;

		int stride() const;

		//const MeshInfo & getFullMeshInfo() const;

		std::vector<std::string> getAllTextures() const;

		void addMeshInfo(AssimpMeshInfo & meshInfo);
		
		void setVertexArrayObject(GLVertexArrayObject * vao)
		{
			vertexObjectPtr = vao;
		}
		//VAO contains full mesh information.
		inline GLVertexArrayObject * getVAO()
		{
			return this->vertexObjectPtr;
		}

		void setHasNormals(bool normals)
		{
			_hasNormals = normals;
		}
		void setHasTangentBitangent(bool tbt)
		{
			_hasTangentBitangent = tbt;
		}
		void setHasTextureCoordinates(bool txCoord)
		{
			_hasTextureCoordinates  = txCoord;	
		}

		/**
		 * Add a bone and return it's index
		 * @param  boneInfo new bone info to add
		 * @return          index in the array of bones (for parent)
		 */
		int addBone(const AssimpBoneInfo & boneInfo);

		int getBoneId(const std::string & string);

		AssimpBoneInfo getBoneInfo(int id)
		{
			assert(id < _boneInfo.size());
			return _boneInfo[id];
		}

		int getNumBones(){return _boneInfo.size();}
		//Mesh info avaliable if no other
		//transformations / information is needed.
		//(Useful for basic mesh display)
		
		void setRootInverseTransform(const glm::mat4 & rootInverseTransform)
		{
			_rootInverseTransform = rootInverseTransform;
		}
		
		glm::mat4 getRootInverseTransform() const
		{
			return _rootInverseTransform;
		}

		void addAnimation(const AssimpAnimationInfo & info)
		{
			_animations.push_back(info);
		}

		const std::vector<AssimpAnimationInfo> & getAnimations()
		{
			return _animations;
		}

	private:
        //Pointer to the vertex array object that contains the mesh.
        //Most meshes are saved  in MeshLibrary, but not all.
        std::vector<AssimpAnimationInfo> _animations;
		std::vector<AssimpMeshInfo> _meshInfo;
		std::vector<AssimpBoneInfo> _boneInfo;
		std::unordered_map<std::string, int> _boneToIndexMap;
		glm::mat4 _rootInverseTransform;

		BasicMeshInfo fullMeshInfo;
		GLVertexArrayObject * vertexObjectPtr;
		bool _hasNormals;
		bool _hasTangentBitangent;
		bool _hasBones;
		bool _hasTextureCoordinates;
		

	};
	
}
