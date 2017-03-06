#pragma once

#include "GLWrapper/GLProgram.h"
#include "Libraries/Library.h"

namespace MoonEngine {

    class GUI : public Component {
    public:
        GUI();

        void start();
        void update(float dt);
        std::shared_ptr<Component> clone() const;
    private:
        std::unordered_map<std::string, std::shared_ptr<GameObject>> _guiElements;

    };
}