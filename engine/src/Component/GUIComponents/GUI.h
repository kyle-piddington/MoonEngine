#pragma once

#include "GLWrapper/GLProgram.h"
#include "Libraries/Library.h"

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
    };
}