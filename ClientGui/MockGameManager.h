#pragma once
#include <memory>
#include "../TheGame/Game.h" 

class MockGameManager
{
public:
    static MockGameManager& get()
    {
        static MockGameManager instance;
        return instance;
    }

    Game& game() { return *m_game; }

private:
    MockGameManager();

    std::unique_ptr<Game> m_game;
};
