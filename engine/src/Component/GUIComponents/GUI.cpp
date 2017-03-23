#include <iostream>
#include <MoonEngine.h>
#include "GUI.h"


using namespace MoonEngine;

GUI::GUI(float width, float height):
    _width(width),
    _height(height),
    _current_moon(0),
    _stars_collected(0),
    _shardTicker(0.0),
    _moonTicker(0.0),
    _shardSizeChange(1.5),
    _moonSizeChange(3.0),
    _animatingShard(false),
    _animatingMoon(false),
    _num_slideshow(0),
    _cur_slideshow(0),
    _wait(0),
    _checkpoints({
        0.33f,
        0.66f,
        0.99f
    })
{
}
void GUI::addElement(string name, float scaleX, float scaleY, float posX, float posY)
{
    addElement(name, scaleX, scaleY, posX, posY, "gui/");
}

void GUI::addElement(string name, float scaleX, float scaleY, float posX, float posY, string folder)
{
    shared_ptr<GameObject> _guiElement = GetWorld()->createGameObject();
    _guiElement->addTag(T_GUI);

    _guiElement->getTransform().setPosition(glm::vec3(posX, posY, 0));
    _guiElement->getTransform().setScale(glm::vec3(scaleX, scaleY, 0));
    // Apparently images are upside down.
    _guiElement->getTransform().setRotation(glm::vec3(M_PI, 0, 0));

    _guiElement->addComponent(GetWorld()->createComponent<SimpleTexture>(folder + name));
    GetWorld()->addGameObject(_guiElement);

    _guiElements[name] = _guiElement;
}

void GUI::createStringTexture(std::string text)
{
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
    if (_animatingShard) {
        glm::vec3 scale = glm::vec3(100.0 + _shardTicker, 80.0 + _shardTicker, 0.0);
        glm::vec3 rotation = glm::vec3(M_PI, 0.0, sin(_shardTicker) / 10.0);
        _guiElements["gui_shard_glow"]->getTransform().setScale(scale);
        _guiElements["gui_shard_glow"]->getTransform().setRotation(rotation);

        _shardTicker += _shardSizeChange;

        if (_shardTicker > 30.0) {
            _shardSizeChange = -1.5f;
        }
        if (_shardTicker <= 0.0) {
            _animatingShard = false;
            _shardTicker = 0.0;
            _shardSizeChange = 1.5;
            _guiElements["gui_shard_glow"]->setDeleted();
        }
    }
}

void GUI::animateMoonGui()
{
    if (_animatingMoon) {
        glm::vec3 scale = glm::vec3(75.0 + _moonTicker, 75.0 + _moonTicker, 0.0);
        glm::vec3 rotation = glm::vec3(M_PI, 0.0, 0.0);

        int moon = _current_moon;
        if (_moonSizeChange > 0) {
            moon--;
        }
        string currentTexture = "Moon" + std::to_string(moon);

        if (_moonTicker > 20.0 && _moonSizeChange == 3.0) {
            _moonSizeChange = 5.0f;
        }
        if (_moonTicker > 30.0) {
            addElement("Moon" + std::to_string(_current_moon), scale.x, scale.y, 0.1f * _width, 0.7f * _height);
			Delete(_guiElements[currentTexture].get());
            _moonSizeChange = -3.0f;
        }

        _guiElements[currentTexture]->getTransform().setScale(scale);
        _guiElements[currentTexture]->getTransform().setRotation(rotation);

        _moonTicker += _moonSizeChange;

        if (_moonTicker <= 0.0) {
            _animatingMoon = false;
            _moonTicker = 0.0;
            _moonSizeChange = 3.0;
        }
    }
}

void GUI::handleRespawnScreen()
{
    if (_wait > 0)
    {
        _wait--;
        if (_wait <= 1) {
            if (_guiElements["outoftime"] != NULL)
            {
                _guiElements["outoftime"]->setDeleted();
                _guiElements["outoftime"] = NULL;
            }
            if (_guiElements["died"] != NULL)
            {
                _guiElements["died"]->setDeleted();
                _guiElements["died"] = NULL;
            }
            sendGlobalMessage("respawn");
        }
    }
}

void GUI::start()
{
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
    addElement("star_count", 30.0f, -30.0f, 0.145f * _width, 0.885f * _height, "");

    addElement("progress", 0.4f * _width, 25.0f, 0.5f * _width, 0.067f * _height);

    for (int i = 0; i < 3; i++)
    {
        addElement("time/moontime" + std::to_string(i), 40.0f, 40.0f,
                   0.1f * _width + (_width * 0.8f * _checkpoints[i]), 0.067f * _height);
    }
    addElement("wolfmoon", 40.0f, 40.0f, 0.33f * _width, 0.067f * _height);

    for (int i = _num_slideshow - 1; i >= 0; i--)
    {
        //addElement("slideshow" + std::to_string(i), _width / 2, _height / 2, _width / 2, _height / 2);
    }

    addElement("menu", _width / 2, _height / 2, _width / 2, _height / 2);


    on(INTRO_STATE,[&](const Message & msg)
    {
        LOG(INFO, "Removing menu");
        _guiElements["menu"]->setDeleted();

        _wait = 30 * _num_slideshow;
    });

    on("picked_up_shard",[&](const Message & msg)
    {
        _stars_collected++;
        LOG(INFO, "Received global message");

        SimpleTexture * texture = _guiElements["star_count"]->getComponent<SimpleTexture>();
        createStringTexture(to_string(_stars_collected));
        texture->setTexture("star_count");

        if (!_animatingShard)
        {
            addElement("gui_shard_glow", 100.0f, 80.0f, 0.105f * _width, 0.87f * _height);
            _animatingShard = true;
        }
    });

    on("picked_up_moon",[&](const Message & msg)
    {
        addElement("time/fullmoontime" + std::to_string(_current_moon), 40.0f, 40.0f,
                   0.1f * _width + (_width * 0.8f * _checkpoints[_current_moon]), 0.067f * _height);

        _current_moon++;

        if (!_animatingMoon)
        {
            _animatingMoon = true;
        }

        if (_current_moon <= 3)
        {

            /* GAME OVER MAN, GAME OVER */
            if (_current_moon == 3) {
                LOG(INFO, "ENDING GAME");
                GetWorld()->getGameState()->setState(ENDING_STATE);
            }
        }
    });

    on(ENDED_STATE,[&](const Message & msg)
    {
        addElement("endscreen", _width / 2, _height / 2, _width / 2, _height / 2);
    });

    on("out_of_time",[&](const Message & msg)
    {
        addElement("outoftime", _width / 2, _height / 2, _width / 2, _height / 2);
        _wait = 60;
    });

    on(DEAD_STATE,[&](const Message & msg)
    {
        addElement("died", _width / 2, _height / 2, _width / 2, _height / 2);
        _wait = 90;
    });
}

void GUI::update(float dt)
{
    float t = MathUtil::clamp(GetWorld()->getGlobalTime(),0.0f,1.0f);
    glm::vec3 progressPos = _guiElements["wolfmoon"]->getTransform().getPosition();
    _guiElements["wolfmoon"]->getTransform().setPosition(
        glm::vec3(0.1*_width + (_width*0.8 * t), progressPos.y,progressPos.z));

    animateShardGui();
    animateMoonGui();

    if (_num_slideshow > 0 && _wait > 0)
    {
        _wait--;
        if (_wait <= 30 * (_num_slideshow - 1))
        {
            _guiElements["slideshow" + std::to_string(_cur_slideshow)]->setDeleted();
            _num_slideshow--;
            _cur_slideshow++;
        }
    }
    else {
        handleRespawnScreen();
    }
}

std::shared_ptr<Component> GUI::clone() const
{
    return std::make_shared<GUI>(*this);
}
