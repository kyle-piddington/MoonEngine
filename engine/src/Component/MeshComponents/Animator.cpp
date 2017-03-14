#include "Animator.h"
#include "GameObject/GameObject.h"
using namespace MoonEngine;

Animator::Animator():
_speedMultiply(1.0),
_animTime(0),
_currentAnimation(nullptr)
{

}

void Animator::start()
{
	_skeleton = getGameObject()->getComponent<Skeleton>();
	if(_skeleton == nullptr)
	{
		LOG(ERROR, "Animator attached with no skeleton!");
	}
}

void Animator::update(float dt)
{
	
	if(_skeleton && _currentAnimation)
	{
		_animTime += _speedMultiply * dt;
		float tick = _currentAnimation->getTickForTime(_animTime);
		const std::vector<SkeletalAnimation::BoneAnimation> & boneAnims = _currentAnimation->getAnimationData();
		for(const SkeletalAnimation::BoneAnimation & animation : boneAnims)
		{
			Transform cTransform = animation.getTransformAtTick(tick);
			Bone * bone = _skeleton->getBone(animation.getBoneName());
			if(bone)
			{
				bone->setAnimatedTransform(cTransform.getMatrix());
			}
		}
	}

}

void Animator::addAnimation(const SkeletalAnimation & animation)
{
	animations[animation.getAnimationName()] = animation;
}

void Animator::setAnimation(const std::string & animName)
{
	auto itr = animations.find(animName);
	if(itr != animations.end())
	{
		_currentAnimation = &(itr->second);
	}
	else
	{
		LOG(ERROR, "Animation  " + animName + " does not exist");
	}
}

std::shared_ptr<Component> Animator::clone() const
{
	return std::make_shared<Animator>(*this);
}

