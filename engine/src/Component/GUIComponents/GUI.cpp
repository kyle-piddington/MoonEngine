#include <iostream>
#include <MoonEngine.h>
#include "GUI.h"


using namespace MoonEngine;

GUI::GUI()
{
}

void GUI::start() {
    _guiElement = GetWorld()->createGameObject();
    _guiElement->addTag(T_GUI);

    _guiElement->getTransform().setPosition(glm::vec3(50,50,0));
    _guiElement->getTransform().setScale(glm::vec3(100,100,0));

    _guiElement->addComponent(GetWorld()->createComponent<SimpleTexture>("solid_white.png"));
    GetWorld()->addGameObject(_guiElement);

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
