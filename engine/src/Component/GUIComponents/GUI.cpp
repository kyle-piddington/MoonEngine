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

void GUI::createStringTexture(std::string text) {
    int width = 50;
    int height = 50;
    unsigned char * pixels;
    if(!(pixels = (unsigned char *)malloc(width * height * 4))) {
        perror("failed to allocate image memory");
        return;
    }
    memset(pixels, 0x20, width * height * 4);	/* clear fb */

    dtx_target_raster(pixels, width, height);

    dtx_color(1.0, 1.0, 1.0, 1.0);
    dtx_position(5, 5);
    dtx_printf("1234567890!");

    GLTextureConfiguration cfg(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT);
    Library::TextureLib->createTexture("text_test", pixels, cfg);

}

void GUI::start() {
    string path = Library::getResourcePath() + "serif_s24.glyphmap";
    LOG(INFO, "Loading font " + path);

    if(!(font = dtx_open_font_glyphmap(path.c_str()))) {
        LOG(INFO, "Couldn't load font");
    }
    dtx_use_font(font, 24);
    dtx_set(DTX_RASTER_BLEND, 1);

    createStringTexture("");


    addElement("Moon0", 75.0f, 75.0f, 0.1f * _width, 0.7f * _height);
    addElement("star", 40.0f, 40.0f, 0.085f * _width, 0.87f * _height);
    addElement("text_test", 200.0f, 200.0f, 0.35f * _width, 0.80f * _height);

    addElement("progress", 0.4f * _width, 25.0f, 0.5f * _width, 0.067f * _height);
    addElement("wolfmoon", 40.0f, 40.0f, 0.33f * _width, 0.067f * _height);

    addElement("menu", _width / 2, _height / 2, _width / 2, _height / 2);

    on(INTRO_STATE,[&](const Message & msg)
    {
        LOG(INFO, "Removing menu");
        _guiElements["menu"]->setDeleted();
    });

    on("picked_up_star",[&](const Message & msg)
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
