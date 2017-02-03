#pragma once

#include "Scene/Scene.h"
#include "GameObject/GameObject.h"
#include <glm/glm.hpp>

namespace MoonEngine
{
    /**
     * Create a copy of this game object at the game object's location
     * @param objToCopy the object to copy.
     */
    std::shared_ptr<GameObject> Instantiate(GameObject * objToCopy);

    /**
     * Create a copy of this game object at a new position and rotation
     */
    std::shared_ptr<GameObject> Instantiate(GameObject * object, glm::vec3 position, glm::vec3 rotation);

    void Delete(GameObject * object);

    /**
     * Get the active Scene
     * @return [description]
     */
    Scene * GetWorld();

    /**
     * Set the active scene that holds game objects to a scene
     * @param scene the scene to add game objects to.
     */
    void SetActiveScene(Scene * scene);


}