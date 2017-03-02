#include <iostream>
#include "GUI.h"


using namespace MoonEngine;

GUI::GUI()
{
}

void GUI::start() {
    on("picked_up_star",[&](const Message & msg)
    {
        std::cout << "Received global message" << endl;
    });
}

void GUI::update(float dt)
{

}

std::shared_ptr<Component> GUI::clone() const
{
    return std::make_shared<GUI>(*this);
}
