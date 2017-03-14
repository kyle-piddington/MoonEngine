#include "Skeleton.h"
#include "Util/Logger.h"
#include <glm/gtc/type_ptr.hpp>

using namespace MoonEngine;

Bone::Bone(std::string name, int boneIdx, glm::mat4 offsetTransform):
        name(name),
        offsetTransform(offsetTransform),
        localAnimTransform(1.0),
        animTransform(1.0),
        idx(boneIdx)
{

}



void Bone::setAnimatedTransform(glm::mat4 transform)
{
    this->localAnimTransform = transform;
}

Skeleton::Skeleton():
Component()
{

}
Skeleton::Skeleton(AssimpModelInfo & info):
Component()
{
    importBonesFromAssimp(info);
}
Skeleton::~Skeleton()
{

}

int Skeleton::getNumBones()
{
    return bones.size();
}

void Skeleton::importBonesFromAssimp(AssimpBoneInfo & node, AssimpModelInfo & info, BoneTreeNode & thisNode)
{
    int boneIdx = addBone(node);
    thisNode.boneIdx = boneIdx;
    for(int i = 0; i < node.childBones.size(); i++)
    {
        thisNode.children.push_back(BoneTreeNode());
        AssimpBoneInfo childInfo = info.getBoneInfo(node.childBones[i]);
        importBonesFromAssimp(childInfo, info, thisNode.children.back());
    }


}
void Skeleton::importBonesFromAssimp(AssimpModelInfo & importInfo)
{

    rootInverseTransform = importInfo.getRootInverseTransform();
    AssimpBoneInfo rootInfo = importInfo.getBoneInfo(0); 
    boneRoot.children.push_back(BoneTreeNode());
    importBonesFromAssimp(rootInfo,importInfo, boneRoot.children.back());

}
Bone * const Skeleton::getBone(std::string boneName)
{
    auto boneId = boneMap.find(boneName);
    if(boneId == boneMap.end())
    {
        LOG(ERROR,  "No Bone named " + boneName + "In skeleton!");
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
int Skeleton::addBone(AssimpBoneInfo & info)
{
    LOG(INFO, "Adding bone " + info.boneName);
    Bone bone(info.boneName,bones.size(),info.offsetMatrix);
    bones.push_back(bone);
    boneMap[info.boneName] = bone.getIndex();
    return bone.getIndex();

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

std::shared_ptr<Component> Skeleton::clone() const
{
    return std::make_shared<Skeleton>(*this);
}
