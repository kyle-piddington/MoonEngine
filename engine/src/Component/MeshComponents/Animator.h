#pragma once
#include "Component/Component.h"
#include "Skeleton.h"
#include "SkeletalAnimation.h"

namespace MoonEngine
{
	class Animator: public Component
	{
	public:
		Animator();

		void start();

		void update(float dt);

		void addAnimation(const SkeletalAnimation & animation);

		void setAnimation(const std::string  & animationName);

		void setAnimTime(float time);

		void setAnimSpeed(float speedMultiply);

		std::vector<std::string> getAnimations() const;

		std::shared_ptr<Component> clone() const;
	private:
		std::unordered_map<std::string, SkeletalAnimation> animations;
		float _animTime;
		float _speedMultiply;
		SkeletalAnimation * _currentAnimation;
		Skeleton * _skeleton;


	};
}