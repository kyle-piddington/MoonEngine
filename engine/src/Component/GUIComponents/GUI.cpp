#include <iostream>
#include <MoonEngine.h>
#include "GUI.h"


using namespace MoonEngine;

GUI::GUI(float width, float height):
    _width(width),
    _height(height),
    _current_moon(0)
{
}

void GUI::addElement(string name, float scaleX, float scaleY, float posX, float posY) {
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
    addElement("Moon0", 75.0f, 75.0f, 0.1f * _width, 0.7f * _height);
    addElement("star", 40.0f, 40.0f, 0.085f * _width, 0.87f * _height);
    addElement("text", 25.0f, 25.0f, 0.135f * _width, 0.88f * _height);

    addElement("progress", 0.4f * _width, 25.0f, 0.5f * _width, 0.067f * _height);
    addElement("wolfmoon", 40.0f, 40.0f, 0.5f * _width, 0.067f * _height);

    on("picked_up_shard",[&](const Message & msg)
    {
        LOG(INFO, "Received global message");
    });

    on("picked_up_moon",[&](const Message & msg)
    {
        _current_moon++;
        if (_current_moon <= 3)
        {
            addElement("Moon" + std::to_string(_current_moon), 75.0f, 75.0f, 0.1f * _width, 0.7f * _height);
        }
    });
}

void GUI::update(float dt)
{

}

std::shared_ptr<Component> GUI::clone() const
{
    return std::make_shared<GUI>(*this);
}
