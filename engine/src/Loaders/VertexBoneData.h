#pragma once
#define NUM_BONES_PER_VERTEX 4
#include <cassert>

namespace MoonEngine
{
//Global number of bones per vertex
	struct VertexBoneData
	{

		unsigned int boneIds[NUM_BONES_PER_VERTEX];
		float boneWeights[NUM_BONES_PER_VERTEX];

		VertexBoneData()
		{
			for(int i = 0; i < NUM_BONES_PER_VERTEX; i++)
			{
				boneIds[i] = 0;
				boneWeights[i] = 0.0f;
			}
		}
		void addBoneData(int Bone, float weight)
		{
			for(unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++)
			{
				if(boneWeights[i] == 0.0){
					boneIds[i] = Bone;
					boneWeights[i] = weight;

					return;
				}
			}

      		assert(false); //Too many bones!
 		}
		bool isEmpty()
		{
		  	return boneWeights[0] == 0.0;
		}

	};

}