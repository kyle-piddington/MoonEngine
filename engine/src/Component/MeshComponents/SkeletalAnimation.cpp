#include "SkeletalAnimation.h"
#include <cassert>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace MoonEngine;

//Small tracker for naming unnamed animations
int SkeletalAnimation::numImported = 0;

SkeletalAnimation::PositionKeyFrame
SkeletalAnimation::PositionKeyFrame::interpolate(const PositionKeyFrame & f1, const PositionKeyFrame & f2, float time)
{
    float dt = f2.startTime - f1.startTime;
    float t = (time - f1.startTime)/dt;
    glm::vec3 interpolatedPosition = (1-t) * f1.getPosition()  + t * f2.getPosition();
    SkeletalAnimation::PositionKeyFrame newFrame(interpolatedPosition,time);
    return newFrame;
}
SkeletalAnimation::RotationKeyFrame
SkeletalAnimation::RotationKeyFrame::interpolate(const RotationKeyFrame & f1, const RotationKeyFrame & f2, float time)
{
    float dt = f2.startTime - f1.startTime;
    float t = (time - f1.startTime)/dt;
    glm::quat interpolatedQuaternion = glm::slerp(f1.getRotation(),f2.getRotation(),t);
    SkeletalAnimation::RotationKeyFrame newFrame(glm::normalize(interpolatedQuaternion),time);
    return newFrame;
}

SkeletalAnimation::ScaleKeyFrame
SkeletalAnimation::ScaleKeyFrame::interpolate(const ScaleKeyFrame & f1, const ScaleKeyFrame & f2, float time)
{
    float dt = f2.startTime - f1.startTime;
    float t = (time - f1.startTime)/dt;
    glm::vec3 interpolatedScale    = (1-t) * f1.getScale() + t * f2.getScale();
    SkeletalAnimation::ScaleKeyFrame newFrame(interpolatedScale,time);
    return newFrame;
}

/**
 * Interpolate the positional keyframe for a tick
 */
glm::vec3 SkeletalAnimation::BoneAnimation::getTranslationAtTick(float tick) const
{
    assert(posFrames.size() > 0);
    if(posFrames.size() == 1)
    {
        return posFrames[0].getPosition();
    }
    else
    {
        PositionKeyFrame frame1 = posFrames[0];
        PositionKeyFrame frame2 = posFrames[0];
        for (std::vector<const PositionKeyFrame>::iterator i = posFrames.begin() + 1; i != posFrames.end(); ++i)
        {
            frame1 = frame2;
            frame2 = *i;
            if(i->getStartTime() > tick)
                break;
        }
        return PositionKeyFrame::interpolate(frame1,frame2,tick).getPosition();
    }
}

/**
 * Interpolate the rotational keyframe for a tick.
 */
glm::quat SkeletalAnimation::BoneAnimation::getRotationAtTick(float tick) const
{
    assert(rotFrames.size() > 0);
    if(rotFrames.size() == 1)
    {
        return rotFrames[0].getRotation();
    }
    else
    {
        RotationKeyFrame frame1 = rotFrames[0];
        RotationKeyFrame frame2 = rotFrames[0];
        for (std::vector<const RotationKeyFrame>::iterator i = rotFrames.begin() + 1; i != rotFrames.end(); ++i)
        {
            frame1 = frame2;
            frame2 = *i;
            if(i->getStartTime() > tick)
                break;
        }
        return RotationKeyFrame::interpolate(frame1,frame2,tick).getRotation();
    }
}

/**
 * Interpolate the scale keyframe for a tick
 */
glm::vec3 SkeletalAnimation::BoneAnimation::getScaleAtTick(float tick) const
{
    assert(scaleFrames.size() > 0);
    if(scaleFrames.size() == 1)
    {
        return scaleFrames[0].getScale();
    }
    else
    {
        ScaleKeyFrame frame1 = scaleFrames[0];
        ScaleKeyFrame frame2 = scaleFrames[0];
        for (std::vector<const ScaleKeyFrame>::iterator i = scaleFrames.begin() + 1; i != scaleFrames.end(); ++i)
        {
            frame1 = frame2;
            frame2 = *i;
            if(i->getStartTime() > tick)
                break;
        }
        return ScaleKeyFrame::interpolate(frame1,frame2,tick).getScale();
    }
}



Transform SkeletalAnimation::BoneAnimation::getTransformAtTick(float tick)  const
{
    Transform transform;
    transform.setPosition(getTranslationAtTick(tick));
    transform.setRotation(getRotationAtTick(tick));
    transform.setScale(getScaleAtTick(tick));
    return transform;
}
void SkeletalAnimation::BoneAnimation::addFrame(PositionKeyFrame posFrame)
{
    if(posFrames.empty())
    {
        posFrames.push_back(posFrame);
    }
    else
    {
        auto i = posFrames.end();
        for(; i != posFrames.begin(); i--)
        {
            if((i-1)->getStartTime() < posFrame.getStartTime())
                break;

        }
        posFrames.insert(i,posFrame);
    }
}
void SkeletalAnimation::BoneAnimation::addFrame(RotationKeyFrame rotFrame)
{
    if(rotFrames.empty())
    {
        rotFrames.push_back(rotFrame);
    }
    else
    {
        auto i = rotFrames.end();
        for(; i != rotFrames.begin(); i--)
        {
            if((i-1)->getStartTime() < rotFrame.getStartTime())
                break;

        }
        rotFrames.insert(i,rotFrame);
    }
}
void SkeletalAnimation::BoneAnimation::addFrame(ScaleKeyFrame scaleFrame)
{
    if(scaleFrames.empty())
    {
        scaleFrames.push_back(scaleFrame);

    }
    else
    {
        auto i = scaleFrames.end();
        for(; i != scaleFrames.begin(); i--)
        {
            if((i-1)->getStartTime() < scaleFrame.getStartTime())
                break;

        }
        scaleFrames.insert(i,scaleFrame);
    }
}

float SkeletalAnimation::getTickForTime(float time) const
{
    return fmod(time * ticksPerSecond, tickDuration);

}

SkeletalAnimation SkeletalAnimation::importFromAssimp(aiAnimation * aiAnim)
{
    SkeletalAnimation sAnim;
    sAnim.name = std::string(aiAnim->mName.data);
    if(sAnim.name.empty())
    {
        sAnim.name = "imported_" + std::to_string(SkeletalAnimation::numImported);
        SkeletalAnimation::numImported++;
    }
    sAnim.tickDuration = aiAnim->mDuration;
    sAnim.ticksPerSecond = aiAnim->mTicksPerSecond != 0 ? aiAnim->mTicksPerSecond : 25.0f;
    for(int bone = 0; bone < aiAnim->mNumChannels; bone++)
    {
        aiNodeAnim * nodeAnim = aiAnim->mChannels[bone];
        BoneAnimation boneAnim(std::string(nodeAnim->mNodeName.data));
        for(int transKeys = 0; transKeys < nodeAnim->mNumPositionKeys; transKeys++)
        {
            aiVectorKey key = nodeAnim->mPositionKeys[transKeys];
            PositionKeyFrame frame(
                    glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z),
                    key.mTime);
            boneAnim.addFrame(frame);
        }
        for(int rotKeys = 0; rotKeys < nodeAnim->mNumRotationKeys; rotKeys++)
        {
            aiQuatKey  key = nodeAnim->mRotationKeys[rotKeys];
            RotationKeyFrame frame(
                    glm::quat(key.mValue.w,key.mValue.x,key.mValue.y,key.mValue.z),
                    key.mTime);
            boneAnim.addFrame(frame);
        }
        for(int scaleKeys = 0; scaleKeys < nodeAnim->mNumScalingKeys; scaleKeys++)
        {
            aiVectorKey key = nodeAnim->mScalingKeys[scaleKeys];
            ScaleKeyFrame frame(glm::vec3(key.mValue.x,key.mValue.y,key.mValue.z),key.mTime);
            boneAnim.addFrame(frame);
        }
        std::cout << "adding frames for " << boneAnim.getBoneName() << " " << nodeAnim->mNumPositionKeys << " Translational, " << nodeAnim->mNumRotationKeys << " Rotational, " << nodeAnim->mNumScalingKeys << " Scale keys" << std::endl;
        sAnim.boneAnimations.push_back(boneAnim);
    }
    return sAnim;

}