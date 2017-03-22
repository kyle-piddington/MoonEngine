#pragma once

#include <Component/Component.h>

namespace MoonEngine
{
#define MENU_STATE "menu_state"
#define INTRO_STATE "intro_state"
#define PLAYING_STATE "playing_state"
#define RESPAWNING_STATE "respawning_state"
#define DEAD_STATE "dead_state"
#define ENDING_STATE "ending_state"
#define ENDED_STATE "ended_state"

    class GameState : public Component
    {
    public:
        GameState();

        void start();

        void update(float dt);

        std::shared_ptr<Component> clone() const;

        std::string currentState(){return _currentState;}
        void setState(std::string state);

    private:
        std::string _currentState;

    };
}