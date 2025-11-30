#include "GameManager.h"
#include <stdexcept>

GameManager& GameManager::getInstance()
{
    static GameManager instance;
    return instance;
}

bool GameManager::createLobby(int lobbyId, const std::string& hostName)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    if (m_lobbies.find(lobbyId) != m_lobbies.end())
        return false;

    m_lobbies.emplace(lobbyId, Lobby{ lobbyId, hostName });
    return true;
}

bool GameManager::joinLobby(int lobbyId, const std::string& playerName)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end())
        return false;

    return it->second.addPlayer(playerName);
}

Lobby& GameManager::getLobby(int lobbyId)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end())
        throw std::out_of_range("Lobby with ID " + std::to_string(lobbyId) + " not found.");

    return it->second;
}