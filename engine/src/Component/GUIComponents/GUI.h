#pragma once

#include "GLWrapper/GLProgram.h"
#include "Libraries/Library.h"
#include "thirdparty/text/drawtext.h"

namespace MoonEngine {

    class GUI : public Component {
    public:
        GUI(float width, float height);

        void start();
        void update(float dt);
        std::shared_ptr<Component> clone() const;
    private:
        float _width, _height;
        int _current_moon;
        std::unordered_map<std::string, std::shared_ptr<GameObject>> _guiElements;

        void addElement(string name, float scaleX, float scaleY, float posX, float posY);

        struct dtx_font *font;

        void createStringTexture(string text);

        void animateShardGui();
        void animateMoonGui();

        float _shardTicker;
        bool _animatingShard;
        float _shardSizeChange;

        float _moonTicker;
        bool _animatingMoon;
        float _moonSizeChange;

        int _stars_collected;

    };
}