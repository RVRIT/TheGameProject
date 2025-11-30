#include "MockGameManager.h"

MockGameManager::MockGameManager()
{
    std::vector<std::string_view> players = {
        "Player1", "Bot1", "Bot2"
    };

    m_game = std::make_unique<Game>(players);
}
