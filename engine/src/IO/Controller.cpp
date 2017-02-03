#include "Controller.h"
#include "Util/Logger.h"
#include <GLFW/glfw3.h>

using namespace MoonEngine;
enum BTNStatus
{
    PRESS = 1,
    HOLD = 2,
    RELEASE = -1,
    RELEASED = -2

};

short * Controller::_btnStatus = nullptr;
const float * Controller::_axisStatus = nullptr;
bool * Controller::_btnToggles = nullptr;
int Controller::_numButtons;
int Controller::_numAxes;
int      Controller::_joyID;
bool     Controller::_connected;


void Controller::resetStorage()
{
    if (_btnStatus != nullptr)
        delete[] _btnStatus;
    if (_btnToggles != nullptr)
        delete[] _btnToggles;
    glfwGetJoystickAxes(_joyID, &_numAxes);
    glfwGetJoystickButtons(_joyID, &_numButtons);
    _btnStatus = new short[_numButtons];
    _btnToggles = new bool[_numButtons];


}

void Controller::Connect(int joyID)
{
    _joyID = joyID;
    resetStorage();
    _connected = true;
}

void Controller::Disconnect()
{
    if (_btnStatus != nullptr)
        delete[] _btnStatus;
    if (_btnToggles != nullptr)
        delete[] _btnToggles;
    _btnStatus = nullptr;
    _btnToggles = nullptr;
    _joyID = -1;
    _connected = false;
}


bool Controller::IsConnected()
{
    return _connected;
}

void Controller::Update()
{
    _axisStatus = glfwGetJoystickAxes(_joyID, &_numAxes);
    const unsigned char * btns = glfwGetJoystickButtons(_joyID, &_numButtons);

    for (int i = 0; i < _numButtons; i++)
    {
        if (btns[i] == GLFW_PRESS)
        {
            if (_btnStatus[i] == PRESS)
            {
                _btnStatus[i] = HOLD;
            }
            else
            {
                _btnStatus[i] = PRESS;
                _btnToggles[i] = !_btnToggles[i];
            }
        }
        else
        {
            if (_btnStatus[i] == RELEASE)
            {
                _btnStatus[i] = RELEASED;
            }
            else
            {
                _btnStatus[i] = RELEASE;
            }
        }
    }
}

float Controller::GetAxis(unsigned axisNumber)
{
    if (axisNumber >= _numAxes)
    {
        return 0;
    }
    return _axisStatus[axisNumber];
}

bool Controller::IsButtonDown(unsigned buttonNumber)
{
    if (buttonNumber >= _numButtons)
    {
        return false;
    }
    return _btnStatus[buttonNumber] == PRESS;
}

bool Controller::IsButtonUp(unsigned buttonNumber)
{
    if (buttonNumber >= _numButtons)
    {
        return false;
    }
    return _btnStatus[buttonNumber] == RELEASE;
}

bool Controller::Button(unsigned buttonNumber)
{
    if (buttonNumber >= _numButtons)
    {
        return false;
    }
    return _btnStatus[buttonNumber] >= PRESS;
}


