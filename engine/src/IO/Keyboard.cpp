#include "Keyboard.h"
#include <GLFW/glfw3.h>
#include <iostream>

using namespace MoonEngine;
enum Keystatus
{
    PRESS = 1,
    HOLD = 2,
    RELEASE = -1,
    RELEASED = -2
};

short Keyboard::keyStatus[NUM_KEYS] = {RELEASE};
short Keyboard::bfrKeyStatus[NUM_KEYS] = {RELEASE};
bool  Keyboard::keyToggles[NUM_KEYS] = {false};

bool Keyboard::isKeyDown(int key)
{
    return keyStatus[key] >= PRESS;
}

bool Keyboard::isKeyUp(int key)
{
    return keyStatus[key] == RELEASE;
}

bool Keyboard::key(int key)
{
    return keyStatus[key] == PRESS;
}

bool Keyboard::isKeyToggled(int key)
{
    return keyToggles[key];
}

void Keyboard::setKeyStatus(int key, int action)
{
    if (action == GLFW_PRESS)
    {
        bfrKeyStatus[key] = PRESS;
        keyToggles[key] = !keyToggles[key];
    }
    else if (action == GLFW_RELEASE)
    {
        bfrKeyStatus[key] = RELEASE;
    }
}

void Keyboard::update()
{
    for (int i = 0; i < NUM_KEYS; i++)
    {
        if (bfrKeyStatus[i] == PRESS)
        {
            if (keyStatus[i] == PRESS)
            {
                keyStatus[i] = HOLD;
                bfrKeyStatus[i] = HOLD;
            }
            else
            {
                keyStatus[i] = PRESS;
            }
        }
        else if (bfrKeyStatus[i] == RELEASE)
        {
            if (keyStatus[i] == RELEASE)
            {
                keyStatus[i] = RELEASED;
            }
            else
            {
                keyStatus[i] = RELEASE;
            }

        }

    }
}