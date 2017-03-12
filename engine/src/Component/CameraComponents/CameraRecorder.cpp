#include <iostream>
#include <Geometry/Transform.h>
#include <MoonEngine.h>
#include "CameraRecorder.h"

using namespace MoonEngine;

CameraRecorder::CameraRecorder()
{

}

void CameraRecorder::start()
{

}


void CameraRecorder::update(float dt)
{
}

std::shared_ptr<Component> CameraRecorder::clone() const
{
    return std::make_shared<CameraRecorder>(*this);
}