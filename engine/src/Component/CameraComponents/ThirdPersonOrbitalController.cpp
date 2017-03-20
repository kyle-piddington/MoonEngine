#include "ThirdPersonOrbitalController.h"
#include "GameObject/GameObject.h"
#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "IO/Input.h"
#include "Geometry/World.h"
#include "Util/Logger.h"
#include "GlobalFuncs/GlobalFuncs.h"
#include "Util/MathUtil.h"
//Windows C++
#ifndef M_PI
#define M_PI 3.141592653589793
#endif

#include <cmath>
#include <algorithm>

using namespace MoonEngine;

enum CameraStates
{
    NORMAL,
    ROTATEBACK,
    COLLISIONS
};

ThirdPersonOrbitalController::ThirdPersonOrbitalController(float Cam_Move_Speed, float CamSensitvity):
    _CamMoveSpeed(Cam_Move_Speed),
    _CamSensitivity(CamSensitvity),
    _trac(0.0f, 0.0f, 3.0f),
    _targ(0.0f),
    _phi(M_PI / 3),
    _theta(0.0f),
    _distance(2.5f),
    _state(NORMAL),
    _camHeight(0.15),
    _baseCamHeight(0.25)
{
}

void ThirdPersonOrbitalController::start()
{
    player = GetWorld()->findGameObjectWithTag(T_Player);
    assert(player != nullptr);
    _targInterp = _targ = player->getTransform().getPosition();
  
    _tracInterp = _trac = _targ + _distance * glm::vec3(sinf(_phi) * cosf(_theta), cosf(_phi) + _camHeight, sinf(_phi) * sinf(_theta));
    Transform & transform = gameObject->getTransform();
    transform.setPosition(_trac);
    transform.lookAt(_targ);

    auto terrainGO = GetWorld()->findGameObjectWithComponent<Terrain>();
    if(terrainGO != nullptr)
    {
        _terrain = terrainGO->getComponent<Terrain>();
    }
    
 
    //Look for player by default
    //
    LOG(GAME, "Camera started");
}

void ThirdPersonOrbitalController::update(float dt)
{
    glm::vec3 rotate(0.0);

    rotate.y = (Mouse::getLastY() - Mouse::getY()) * _CamSensitivity;
    rotate.x = (Mouse::getLastX() - Mouse::getX()) * _CamSensitivity * -1.0;
    rotate.y -= Input::GetAxis(AXIS_VERTICAL_1) * _CamMoveSpeed * dt;
    rotate.x -= Input::GetAxis(AXIS_HORIZONTAL_1) * _CamMoveSpeed * dt;
    _targ = player->getTransform().getPosition();
    float camHeightScalar = 0.85 * _distance;
    if(_phi > M_PI/2)
    {
        float targHeight = _baseCamHeight + camHeightScalar * (_phi - M_PI/2);   
        _targ = player->getTransform().getPosition() + glm::vec3(0,targHeight,0);     
    }
    else
    {
        _targ = player->getTransform().getPosition();
    }
    if (std::abs(rotate.y) > 1e-2 || std::abs(rotate.x) > 1e-2)
    {
        _phi += rotate.y;
        _theta += rotate.x;

        /* Do not look beyond straight up */
        /* TODO allow looking all the way up.
         * M_PI - 0.1f
         * */
        _phi = std::min(_phi, (float)M_PI*3/4.0f);

        /* Do not look beyond straight down */
        _phi = std::max(_phi, 0.1f);

        rotate.z = 0;

        _trac = _targ + _distance * glm::vec3(sinf(_phi) * cosf(_theta), cosf(_phi), sinf(_phi) * sinf(_theta));

    }
    else if (Input::GetButtonDown(BUTTON_3))
    {
        glm::vec3 fwd = player->getTransform().forward();
        glm::vec3 camDirection = glm::normalize(
                glm::vec3(-fwd.x, 0, -fwd.z));

        _phi = M_PI / 3;
        _camHeight = _baseCamHeight;
        _theta = atan2(camDirection.z, camDirection.x);
        _trac = _targ + _distance * glm::vec3(sinf(_phi) * cosf(_theta), cosf(_phi), sinf(_phi) * sinf(_theta))
                 + glm::vec3(0,_camHeight,0);


    }
    else if (glm::length(gameObject->getTransform().getPosition() - player->getTransform().getPosition()) > _distance + 0.05)
    {
        glm::vec3 camPosition = (glm::vec3(0,-_camHeight,0) + player->getTransform().getPosition());
        glm::vec3 camDirection = glm::normalize(
                gameObject->getTransform().getPosition() - player->getTransform().getPosition());
        _phi = acos(glm::dot(camDirection, glm::normalize(glm::vec3(World::Up))));
        _theta = atan2(camDirection.z, camDirection.x);
        _trac = _targ + glm::normalize(
                gameObject->getTransform().getPosition() - camPosition) * _distance;
    }

    glm::vec3 finalTrac = boundHeight(_trac);
    _tracInterp += (finalTrac - _tracInterp) * dt * 5.0f;
    _targInterp += (_targ - _targInterp) * dt * 5.0f;
    Transform & transform = gameObject->getTransform();
    transform.setPosition(_tracInterp);


    //_targ = transform.getPosition();
    transform.lookAt(_targInterp);

}


glm::vec3 ThirdPersonOrbitalController::boundHeight(glm::vec3 _trac)
{
    float finalY = _trac.y;
    if(_terrain != nullptr)
    {
        const float camThreshhold = 0.3f;
        float _minHeight = _terrain->heightAt(_trac.x, _trac.z) + _camHeight;
       
        float distToGround = _trac.y - _minHeight;
        if(distToGround < camThreshhold)
        {
            float u = distToGround/camThreshhold;
            u = u*u;
            finalY = std::max(_minHeight,  _trac.y);
        }
    }
    return glm::vec3(_trac.x, finalY, _trac.z);
    
}

std::shared_ptr<Component> ThirdPersonOrbitalController::clone() const
{
    return std::make_shared<ThirdPersonOrbitalController>(*this);
}