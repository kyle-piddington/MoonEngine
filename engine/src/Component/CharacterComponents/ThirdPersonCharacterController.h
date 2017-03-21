#pragma once

#include "Component/Component.h"
#include "Component/CollisionComponents/BoxCollider.h"
#include "Component/MeshComponents/Terrain.h"
#include "Component/MeshComponents/Animator.h"

#include "Geometry/Transform.h"
/**
 * Controls a character in a first-person matter
 */
namespace MoonEngine
{
    enum PlayerState
    {
        JUMPING,
        GROUND,
        FALLING
    };

    class ThirdPersonCharacterController: public Component
    {
    public:
        ThirdPersonCharacterController(float playerSpeed = 1);

        void start();

        void update(float dt);

        void onCollisionEnter(Collision col);

        std::shared_ptr<Component> clone() const;

    private:
        void handleMove(float dt);

        void handleJump(float dt);

        void checkIfShouldFall();
        void findMinGround();

        void updateAnimation();
        Transform * transform;
        float playerSpeed;
        float jumpSpeed;
        float jumpForce;
        float jumpTime;
        float _curJumpForce;

        int _currentAnim;
        
        float _jumpTime;
        float gravity;
        float _lastGround;
        PlayerState state;
        float radius;
        GameObject * mainCamera;
        Terrain * worldTerrain;
        Animator * animator;
        BoxCollider * bbox;
		float wispTime;
		int numWisps;

        #define ANIM_CREEP 0
        #define ANIM_IDLE 1
        #define ANIM_WALK 2
        #define ANIM_RUN 3
        #define ANIM_SEAT 4

        string animations[5] = {
                "run2|Wolf_creep_cycle",
                "run2|Wolf_Idle_",
                "run2|Wolf_Walk_cycle_",
                "run2|Wolf_Run_Cycle_",
                "run2|Wolf_seat_"
        };

    };
}