#include <iostream>
#include <MoonEngine.h>
#include "GUI.h"


using namespace MoonEngine;

GUI::GUI(int width, int height):
    _width(width),
    _height(height)
{
}

void GUI::addElement(string name, int scaleX, int scaleY, int posX, int posY) {
    shared_ptr<GameObject> _guiElement = GetWorld()->createGameObject();
    _guiElement->addTag(T_GUI);

    _guiElement->getTransform().setPosition(glm::vec3(posX, posY, 0));
    _guiElement->getTransform().setScale(glm::vec3(scaleX, scaleY, 0));
    // Apparently images are upside down.
    _guiElement->getTransform().setRotation(glm::vec3(M_PI, 0, 0));

    _guiElement->addComponent(GetWorld()->createComponent<SimpleTexture>(name));
    GetWorld()->addGameObject(_guiElement);

    _guiElements[name] = _guiElement;
}

void GUI::start() {
    addElement("Moon2", 75, 75, _width / 10, 17 * _height / 20);
    addElement("star", 50, 50, 9 * _width / 10, 35 * _height / 40);
    addElement("text", 25, 25, 19 * _width / 20, 71 * _height / 80);

    addElement("progress", 2 * _width / 5, 25, _width / 2, _height / 15);
    addElement("wolfmoon", 40, 40, _width / 2, _height / 15);

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
