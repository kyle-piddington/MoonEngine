#pragma once

#include <glm/glm.hpp>
#include "World.h"
#include <glm/gtc/quaternion.hpp>

namespace MoonEngine
{
    namespace Space
    {
        enum spaceType
        {
            WORLD = 0,
            LOCAL = 1
        };
    }
    class Transform
    {
    public:
        Transform();

        /**
        * Set the position of the transform
        * @param pos position to go to
        */
        void setPosition(const glm::vec3 & pos);

        /**
        * Copy a transform from another transofrm
        * @param other the transform to copy.
        */
        void copy(Transform & other);

        /**
        * Set the rotation of the transform
        * @param eulerAngles the angles to rotate to
        */
        void setRotation(const glm::vec3 & eulerAngles);

        /**
        * Set the rotation using Angle-Axis
        * @param angle the angle
        * @param axis  the axis to rotate on
        */
        void setRotation(float angle, const glm::vec3 & axis);

        /* Set the rotation using a straight quaternion
        * @param angle the angle
        * @param axis  the axis to rotate on
        */
        void setRotation(const glm::quat & q);

        /**
        * Move by a vector
        * @param pos the delta vector
        */
        void translate(const glm::vec3 & pos, Space::spaceType type = Space::WORLD);

        /**
        * Rotate by a set of angles
        * @param eulerAngles the new orientation to rotate to
        */
        void rotate(const glm::vec3 eulerAngles, Space::spaceType type = Space::WORLD);

        /**
        * Rotate by a set of angles
        * @param eulerAngles the new orientation to rotate to
        */
        void rotate(float angle, const glm::vec3 & axis, Space::spaceType type = Space::WORLD);

        /**
        * Orient the transform to point towards a target
        * @param target the target position
        */
        void lookAt(glm::vec3 target, glm::vec3 up = World::Up);

        /**
        * Scale the transform
        */
        void setScale(glm::vec3 scale);

        /**
        * Scale the transform uniformly
        */
        void setScale(float scale);

        /**
        * Get the current position expressed as a 3d vector
        */
        glm::vec3 getPosition(Space::spaceType type = Space::WORLD) const;

        /**
        * Get the current rotation expressed as a quaternion
        * @return [description]
        */
        glm::quat getRotation() const;

        /**
        * Get the scale of the transform
        */
        glm::vec3 getScale() const;

        /**
        * Get a 4x4 transform matrix representing this transform
        */
        const glm::mat4 & getMatrix();

        glm::mat4 getRotationMatrix();

        /**
        * Get the local up facing vector
        */
        glm::vec3 up() const;

        /**
        * Get the local right facing vector
        */
        glm::vec3 right() const;

        /**
        * Get the local forward facing vector
        */
        glm::vec3 forward() const;

        void setParent(Transform * parent)
        {
            this->parent = parent;
        }

    private:
        void updateFrame();

        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        glm::vec4 localUp;
        glm::vec4 localRight;
        glm::vec4 localForward;
        glm::mat4 currentMatrix;
        glm::mat4 finalMatrix;
        bool isDirty;
        Transform * parent;

    };
};