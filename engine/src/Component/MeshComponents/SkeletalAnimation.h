#pragma once

#include <assimp/scene.h>
#include "Geometry/Transform.h"
#include <vector>
#include "Loaders/AssimpModelInfo.h"
namespace MoonEngine
{
/**
 * A Keyframed Skeletal animation, contains a series of bones and keyframes to modify.
 */

    class SkeletalAnimation
    {
    public:
        /**
         * Represents a change of translation of a bone
         */
        class PositionKeyFrame
        {

            glm::vec3 position;
            float startTime;
        public:
            /**
             * Interpolate two keyframes to create a third
             * @param  f1 The first keyframe
             * @param  f2 the second keyframe
             * @param  t  the amount to interpolate
             * @return    a new keyframe
             */
            static PositionKeyFrame interpolate(const PositionKeyFrame & f1, const PositionKeyFrame & f2, float t);

            /**
             * Get the position of the keyrframe in 3d space
             * @return the 3d position in local space
             */
            glm::vec3 getPosition() const
            {
                return position;
            }

            double getStartTime() const
            {
                return startTime;
            }

            PositionKeyFrame(glm::vec3 position, float startTime):
                    position(position),
                    startTime(startTime)
            {}
        };

        /**
         * Represents a rotation of a bone
         * A Quaternion is used to represent a rotation
         */
        class RotationKeyFrame
        {

            glm::quat rotation;
            float startTime;
        public:
            /**
             * Interpolate two rotations using spherical interpolation
             * @param  f1 The first keyframe
             * @param  f2 the second keyframe
             * @param  t  The interpolation percent
             * @return    A new keyframe representing the interpolated postion
             */
            static RotationKeyFrame interpolate(const RotationKeyFrame & f1, const RotationKeyFrame & f2, float t);

            /**
             * Get the rotation of the keyframe
             * @return a quaternion representing a 3d rotation
             */
            glm::quat getRotation() const
            {
                return rotation;
            }

            double getStartTime() const
            {
                return startTime;
            }

            RotationKeyFrame(glm::quat rotation, float startTime):
                    rotation(rotation),
                    startTime(startTime)
            {}
        };

        /**
         * A keyframe representing a scale in a bone
         */
        class ScaleKeyFrame
        {

            glm::vec3 scale;
            float startTime;
        public:
            /**
             * Interpolate between two scale keyframes
             * @param  f1 The first keyframe
             * @param  f2 the second keyframe
             * @param  t  the interpolation percent
             * @return    a new keyframe represnting the interpolated scale
             */
            static ScaleKeyFrame interpolate(const ScaleKeyFrame & f1, const ScaleKeyFrame & f2, float t);

            glm::vec3 getScale() const
            {
                return scale;
            }

            double getStartTime() const
            {
                return startTime;
            }

            ScaleKeyFrame(glm::vec3 scale, float startTime):
                    scale(scale),
                    startTime(startTime)
            {}
        };

        /**
         * A bone animation is a collection of keyframes for a single bone.
         * The bone animation is managed by the SkeletalAnimation class.
         */
        class BoneAnimation
        {
            friend class SkeletalAnimation;

            /**
             * The name of the bone this animation effects
             */
            std::string boneName;

            /**
             * Positional keyframes
             */
            std::vector<PositionKeyFrame> posFrames;
            /**
             * Rotational Keyframes
             */
            std::vector<RotationKeyFrame> rotFrames;
            /**
             * Scale keyframes
             */
            std::vector<ScaleKeyFrame> scaleFrames;

            /**
             * Add a frame. Perofrmance degrades if frames are added out of order
             * @param orientation the orientation of the bone
             * @param time        the time of the frame
             */
            void addFrame(PositionKeyFrame pos);

            void addFrame(RotationKeyFrame pos);

            void addFrame(ScaleKeyFrame pos);

            /**
             * Get a position at a certain tick time. Position will be interpolated between the two nearest keyframes
             * @param  tick The time to interpolate to
             * @return      the position
             */
            glm::vec3 getTranslationAtTick(float tick) const;

            /**
             * Get a rotation at a certain tick time
             * @param  tick The tick to get the translation at
             * @return      a quaternion represnting a rotation
             */
            glm::quat getRotationAtTick(float tick) const;

            /**
             * Get the scale of a bone at a tick time
             * @param  tick the tick to interpolate to
             * @return      a vector representing a scale
             */
            glm::vec3 getScaleAtTick(float tick) const;

        public:

            /**
             * Create a bone animation, with a specific bone
             */
            BoneAnimation(std::string boneName):
                    boneName(boneName)
            {
            }

            /**
             * Get a combined Transform at a certain tick
             * @param  time the tick to get the nimation at
             * @return      a Transformation to bind to a bone
             */
            Transform getTransformAtTick(float time) const;

            /**
             * Get the bone name this animation affects
             * @return the bone name
             */
            std::string getBoneName() const
            {
                return boneName;
            }
        };

        /**
         * Import an animation from an Assimp Animation, and convert it for use with the Skeleton class.
         * @param  anim The animation to import
         * @return      A new SkeletalAnimation with the assimp information
         */
        void importFromAssimp(const AssimpAnimationInfo & aiAnim);

        /**
         * Get all of the animation data for this skeleton
         */
        const std::vector<BoneAnimation> & getAnimationData() const
        {
            return boneAnimations;
        }

        /**
         * Get a tick for a certain timestep.
         * Ticks will be modded to ensure an animation runs smoothly,
         * @param  t A time
         * @return   A tick for use with this animation
         */
        float getTickForTime(float t) const; //Convert time to ticks

        /**
         * Get the animation name
         * @return the given name, or an assigned one.
         */
        std::string getAnimationName() const
        {
            return name;
        }

    private:
        std::vector<BoneAnimation> boneAnimations;
        std::string name;
        float tickDuration;
        float ticksPerSecond;

    };
};
