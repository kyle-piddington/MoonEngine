#include <iostream>
#include <MoonEngine.h>
#include "GUI.h"


using namespace MoonEngine;

GUI::GUI()
{
}

void GUI::start() {
    shared_ptr<GameObject> _guiElement = GetWorld()->createGameObject();
    _guiElement->addTag(T_GUI);

    _guiElement->getTransform().setPosition(glm::vec3(50, 50, 0));
    _guiElement->getTransform().setScale(glm::vec3(100, 100, 0));

    _guiElement->addComponent(GetWorld()->createComponent<SimpleTexture>("solid_white.png"));
    GetWorld()->addGameObject(_guiElement);

    _guiElements["first"] = _guiElement;

    shared_ptr<GameObject> _guiElement2 = GetWorld()->createGameObject();
    _guiElement2->addTag(T_GUI);

    _guiElement2->getTransform().setPosition(glm::vec3(0, 0, 0));
    _guiElement2->getTransform().setScale(glm::vec3(50, 50, 0));

    _guiElement2->addComponent(GetWorld()->createComponent<SimpleTexture>("default.png"));
    GetWorld()->addGameObject(_guiElement2);

    _guiElements["second"] = _guiElement2;

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
