#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/orthonormalize.hpp>

#define GLM_FORCE_RADIANS
using namespace MoonEngine;

Transform::Transform():
    position(glm::vec3(0)),
    rotation(glm::vec3(0.0)),
    scale(glm::vec3(1.0)),
    localUp(glm::vec4(World::Up, 0.0)),
    localRight(glm::vec4(World::Right, 0.0)),
    localForward(glm::vec4(World::Forward, 0.0)),
    currentMatrix(glm::mat4(1.0)),
    isDirty(false),
    parent(nullptr)
{

}

void Transform::setPosition(const glm::vec3 & position)
{
    this->position = position;
    isDirty = true;
}

void Transform::setRotation(const glm::vec3 & eulerAngles)
{

    this->rotation = glm::normalize(glm::quat(glm::vec3(0.0)) * glm::quat(eulerAngles));
    isDirty = true;
    updateFrame();
}

void Transform::setRotation(float angle, const glm::vec3 & axis)
{
    //Assert axis greater than zero
    if (axis.length() == 0)
    {
        assert(!"Axis length of zero on rotation");
    }
    glm::normalize(axis);

    this->rotation = glm::angleAxis(angle, axis);

    isDirty = true;
    //glm::normalize(this->rotation);
    updateFrame();
}

void Transform::setRotation(const glm::quat & quat)
{
    this->rotation = quat;
    isDirty = true;
    updateFrame();
}

void Transform::translate(const glm::vec3 & pos, Space::spaceType type)
{
    glm::vec3 transPosition = pos;
    if (type == Space::LOCAL)
    {
        transPosition = glm::vec3(getMatrix() * glm::vec4(pos, 0.0));
    }
    this->position += transPosition;
    isDirty = true;


}

void Transform::copy(Transform & other)
{
    this->position = other.position;
    this->rotation = other.rotation;
    isDirty = true;
    updateFrame();
}

void Transform::rotate(const glm::vec3 eulerAngles, Space::spaceType type)
{

    this->rotation = glm::normalize(this->rotation * glm::quat(eulerAngles));
    isDirty = true;

    updateFrame();
}

void Transform::rotate(float angle, const glm::vec3 & axis, Space::spaceType space)
{
    if (axis.length() == 0)
    {

        assert("Zero length axis used in angle axis rotation");
    }
    //Convert world axis to local axis
    glm::vec3 localAxis = axis;
    if (space == Space::WORLD)
    {
        glm::mat4 rotMtx = glm::mat4_cast(rotation);
        localAxis = glm::vec3(rotMtx * glm::vec4(axis, 0.0));
    }
    glm::normalize(localAxis);


    glm::quat newQuat = glm::angleAxis(angle, localAxis);
    this->rotation = glm::normalize(this->rotation * newQuat);
    isDirty = true;

    updateFrame();
}

glm::vec3 Transform::getPosition(Space::spaceType type) const
{
    if (type == Space::LOCAL || parent == nullptr)
        return position;
    else
        return glm::vec3(parent->getMatrix() * glm::vec4(position, 1.0));
}

glm::quat Transform::getRotation() const
{
    return rotation;
}

const glm::mat4 & Transform::getMatrix()
{
    if (isDirty)
    {


        glm::mat4 s = glm::mat4(
            this->scale.x, 0, 0, 0,
            0, this->scale.y, 0, 0,
            0, 0, this->scale.z, 0,
            0, 0, 0, 1
        );

        glm::mat4 t = glm::mat4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            this->position.x, this->position.y, this->position.z, 1
        );
        currentMatrix = t * glm::mat4_cast(rotation) * s;
        isDirty = false;

    }
    if (parent != nullptr)
    {
        currentMatrix = parent->getMatrix() * currentMatrix;
    }
    return currentMatrix;
}

void Transform::lookAt(glm::vec3 target, glm::vec3 upVec)
{
    /*Vector forward = lookAt.Normalized();
    Vector right = Vector::Cross(up.Normalized(), forward);
    Vector up = Vector::Cross(forward, right);*/

    glm::vec3 forward = glm::normalize(target - position);
    glm::vec3 up = glm::orthonormalize(upVec, forward); // Keeps up the same, make forward orthogonal to up
    glm::vec3 right = glm::normalize(glm::cross(forward, up));

    glm::mat4 rotMat;
    rotMat[0] = glm::vec4(right.x, right.y, right.z, 0);
    rotMat[1] = glm::vec4(up.x, up.y, up.z, 0);
    rotMat[2] = glm::vec4(-forward.x, -forward.y, -forward.z, 0);
    rotMat[3] = glm::vec4(0, 0, 0, 1);
    //Create a quaternion from the three vectors above.
    glm::quat ret = glm::quat_cast(rotMat);


    this->rotation = glm::normalize(ret);
    isDirty = true;
    updateFrame();
}

void Transform::setScale(glm::vec3 scale)
{
    this->scale = scale;
    isDirty = true;
}

glm::vec3 Transform::up() const
{
    return glm::vec3(localUp);
}

glm::vec3 Transform::right() const
{
    return glm::vec3(localRight);
}

glm::vec3 Transform::forward() const
{
    return glm::vec3(localForward);
}

glm::vec3 Transform::getScale() const
{
    return scale;
}

void Transform::updateFrame()
{

    glm::vec4 wUp(World::Up, 0.0);
    glm::vec4 wRt(World::Right, 0.0);
    glm::vec4 wFw(World::Forward, 0.0);
    glm::mat4 rotMtx = glm::mat4_cast(rotation);
    localRight = glm::normalize(rotMtx * wRt);
    localUp = glm::normalize(rotMtx * wUp);
    localForward = glm::normalize(rotMtx * wFw);
}