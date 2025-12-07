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

bool GameManager::joinLobby(const int lobbyId, const std::string& playerName)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end())
        return false;
    return it->second.addPlayer(playerName);
}

bool GameManager::sendChatMessage(int lobbyId, const std::string& sender, const std::string& content)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) return false;
    return it->second.sendChatMessage(sender, content);
}

std::vector<ChatMessage> GameManager::getChatHistory(int lobbyId) const
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end())
        throw std::out_of_range("Lobby not found");
    return it->second.getChatHistory();
}
//added the game lobby to be fixed soon
Lobby* GameManager::getLobby(int lobbyId)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it != m_lobbies.end()) {
        return &(it->second);
    }
    return nullptr;
}