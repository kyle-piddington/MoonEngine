#pragma once

#include "GLWrapper/GLProgram.h"
#include "Libraries/Library.h"

namespace MoonEngine {

    class GUI : public Component {
    public:
        GUI(int width, int height);

        void start();
        void update(float dt);
        std::shared_ptr<Component> clone() const;
    private:
        int _width, _height;
        std::unordered_map<std::string, std::shared_ptr<GameObject>> _guiElements;

        void addElement(string name, int scaleX, int scaleY, int posX, int posY);
    };
}