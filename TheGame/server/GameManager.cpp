#include "GameManager.h"
#include <stdexcept>

GameManager& GameManager::getInstance()
{
    static GameManager instance;
    return instance;
}

int GameManager::createLobby(const std::string& hostName)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    int newId = m_nextLobbyId++;
    m_lobbies.emplace(newId, Lobby{ newId, hostName });
    return newId;
}

bool GameManager::joinLobby(int lobbyId, const std::string& playerName)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end())
        return false;
    return it->second.addPlayer(playerName);
}
