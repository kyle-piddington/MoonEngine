#pragma once
#define NUM_BONES_PER_VERTEX 4
#include <cassert>

namespace MoonEngine
{
//Global number of bones per vertex
	struct VertexBoneData
	{

		uint boneIds[NUM_BONES_PER_VERTEX];
		float boneWeights[NUM_BONES_PER_VERTEX];
		void addBoneData(int Bone, float weight)
		{
			for(uint i = 0; i < NUM_BONES_PER_VERTEX; i++)
			{
				if(boneWeights[i] == 0.0){
					boneIds[i] = Bone;
					boneWeights[i] = weight;

					return;
				}
			}

      		assert(!"Too many vertices for bone."); //Too many bones!
 		}
		bool isEmpty()
		{
		  	return boneWeights[0] == 0.0;
		}

	};

}