#include "Skeleton.h"
#include <glm/gtc/type_ptr.hpp>

using namespace MoonEngine;

Bone::Bone(std::string name, int boneIdx, glm::mat4 offsetTransform):
        name(name),
        offsetTransform(offsetTransform),
        localAnimTransform(1.0),

        animTransform(1.0),
        idx(boneIdx),
        parentBoneIdx(-1)
{

}



void Bone::setAnimatedTransform(glm::mat4 transform)
{
    this->localAnimTransform = transform;
}

void Bone::setParent(int parentBoneIdx)
{
    this->parentBoneIdx = parentBoneIdx;
}

Skeleton::Skeleton()
{

}
Skeleton::~Skeleton()
{

}

int Skeleton::getNumBones()
{
    return bones.size();
}
void Skeleton::importBonesFromAssimp(aiNode * node, BoneTreeNode & thisNode)
{
    int boneIdx = addBone(std::string(node->mName.data),glm::transpose(glm::make_mat4(&node->mTransformation.a1)));

    thisNode.boneIdx = boneIdx;
    for(int i = 0; i < node->mNumChildren; i++)
    {
        thisNode.children.push_back(BoneTreeNode());
        importBonesFromAssimp(node->mChildren[i],thisNode.children.back());
    }


}
void Skeleton::importBonesFromAssimp(aiNode * node)
{
    rootInverseTransform = glm::transpose(glm::make_mat4(&node->mTransformation.a1));
    rootInverseTransform = glm::inverse(rootInverseTransform);
    for(int i = 0; i < node->mNumChildren; i++)
    {
        boneRoot.children.push_back(BoneTreeNode());
        importBonesFromAssimp(node->mChildren[i],boneRoot.children.back());
    }

}
Bone * const Skeleton::getBone(std::string boneName)
{
    auto boneId = boneMap.find(boneName);
    if(boneId == boneMap.end())
    {
        //LOG(ERROR) << "No Bone named " << boneName << "In skeleton!";
        return nullptr;
    }
    else
    {
        return &bones[boneId->second];
    }
}

/**
 * Returns index of added bone
 */
int Skeleton::addBone(std::string boneName, glm::mat4 boneMtx)
{
    Bone bone(boneName,bones.size(),boneMtx);
    bones.push_back(bone);
    boneMap[boneName] = bone.getIndex();

    return bone.getIndex();

}



void Skeleton::finalize(BoneTreeNode & node, int pIdx)
{
    bones[node.boneIdx].parentBoneIdx = pIdx;
    for (std::vector<BoneTreeNode>::iterator i = node.children.begin(); i != node.children.end(); ++i)
    {
        finalize(*i,node.boneIdx);
    }
}
void Skeleton::finalize()
{
    for (std::vector<BoneTreeNode>::iterator i = boneRoot.children.begin(); i != boneRoot.children.end(); ++i)
    {
        finalize(*i, -1);
    }

}

void Skeleton::finalizeAnimation(BoneTreeNode & node, glm::mat4 parentMtx)
{
    glm::mat4 aMtx = parentMtx * bones[node.boneIdx].localAnimTransform;
    bones[node.boneIdx].animTransform = aMtx;
    for (std::vector<BoneTreeNode>::iterator i = node.children.begin(); i != node.children.end(); ++i)
    {
        finalizeAnimation(*i, aMtx);
    }
}
void Skeleton::finalizeAnimation()
{
    for (std::vector<BoneTreeNode>::iterator i = boneRoot.children.begin(); i != boneRoot.children.end(); ++i)
    {
        finalizeAnimation(*i,rootInverseTransform);
    }
}