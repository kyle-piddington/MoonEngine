#include <iostream>
#include <MoonEngine.h>
#include "GUI.h"


using namespace MoonEngine;

GUI::GUI(float width, float height):
    _width(width),
    _height(height),
    _current_moon(0),
    _stars_collected(0),
    _ticker(0.0),
    _animating(false),
    _size_change(1.5)
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
    int width = 120;
    int height = 120;
    unsigned char * pixels;
    if(!(pixels = (unsigned char *)malloc(width * height * 4))) {
        perror("failed to allocate image memory");
        return;
    }
    memset(pixels, 0x20, width * height * 4);	/* clear fb */

    dtx_target_raster(pixels, width, height);

    dtx_color(1.0, 1.0, 1.0, 1.0);
    dtx_position(0, height / 2);
    dtx_printf(text.c_str());

    GLTextureConfiguration cfg(width / 2, height / 2, GL_RGBA, GL_RGBA, GL_UNSIGNED_SHORT);
    Library::TextureLib->createTexture("star_count", pixels, cfg);

}

void GUI::animateShardGui()
{
    if (_animating) {
        glm::vec3 scale = glm::vec3(100.0 + _ticker, 80.0 + _ticker, 0.0);
        glm::vec3 rotation = glm::vec3(M_PI, 0.0, sin(_ticker) / 10.0);
        _guiElements["gui_shard_glow"]->getTransform().setScale(scale);
        _guiElements["gui_shard_glow"]->getTransform().setRotation(rotation);

        _ticker += _size_change;

        if (_ticker > 30.0) {
            _size_change = -1.5f;
        }
        if (_ticker <= 0.0) {
            _animating = false;
            _ticker = 0.0;
            _size_change = 1.5;
            _guiElements["gui_shard_glow"]->setDeleted();
        }
    }
}

void GUI::start() {
    string path = Library::getResourcePath() + "font_s72.glyphmap";
    LOG(INFO, "Loading font " + path);

    if(!(font = dtx_open_font_glyphmap(path.c_str()))) {
        LOG(INFO, "Couldn't load font");
    }
    dtx_use_font(font, 72);
    dtx_set(DTX_RASTER_BLEND, 1);

    createStringTexture(to_string(_stars_collected));

    addElement("Moon0", 75.0f, 75.0f, 0.1f * _width, 0.7f * _height);
    addElement("gui_shard_flat", 100.0f, 80.0f, 0.105f * _width, 0.87f * _height);
    addElement("star_count", 30.0f, -30.0f, 0.145f * _width, 0.885f * _height);

    addElement("progress", 0.4f * _width, 25.0f, 0.5f * _width, 0.067f * _height);
    addElement("wolfmoon", 40.0f, 40.0f, 0.33f * _width, 0.067f * _height);

    addElement("menu", _width / 2, _height / 2, _width / 2, _height / 2);

    on(INTRO_STATE,[&](const Message & msg)
    {
        LOG(INFO, "Removing menu");
        _guiElements["menu"]->setDeleted();
    });

    on("picked_up_shard",[&](const Message & msg)
    {
        _stars_collected++;
        LOG(INFO, "Received global message");

        SimpleTexture * texture = _guiElements["star_count"]->getComponent<SimpleTexture>();
        createStringTexture(to_string(_stars_collected));
        texture->setTexture("star_count");

        addElement("gui_shard_glow", 100.0f, 80.0f, 0.105f * _width, 0.87f * _height);
        _animating = true;
    });

    on("picked_up_moon",[&](const Message & msg)
    {
        _current_moon++;
        if (_current_moon <= 3)
        {
            addElement("Moon" + std::to_string(_current_moon), 75.0f, 75.0f, 0.1f * _width, 0.7f * _height);

            /* GAME OVER MAN, GAME OVER */
            if (_current_moon == 3) {
                LOG(INFO, "ENDING GAME");
                GetWorld()->getGameState()->setState(ENDING_STATE);
            }
        }
    });

    on(ENDED_STATE,[&](const Message & msg)
    {
        addElement("wolfmoon", 200.0f, 200.0f, _width / 2, _height / 2);
    });
}

void GUI::update(float dt)
{
    float t = MathUtil::clamp(GetWorld()->getGlobalTime(),0.0f,1.0f);
    glm::vec3 progressPos = _guiElements["wolfmoon"]->getTransform().getPosition();
    _guiElements["wolfmoon"]->getTransform().setPosition(
        glm::vec3(0.1*_width + (_width*0.8 * t), progressPos.y,progressPos.z));

    animateShardGui();
}

std::shared_ptr<Component> GUI::clone() const
{
    return std::make_shared<GUI>(*this);
}
