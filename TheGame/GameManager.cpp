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
    m_lobbies.try_emplace(newId, newId, hostName);
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

bool GameManager::setPlayerReady(int lobbyId, int playerId, bool ready)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) {
        return false;
    }
    return it->second.setPlayerReady(playerId, ready);
}

Lobby* GameManager::getLobby(int lobbyId)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it != m_lobbies.end()) {
        return &(it->second);
    }
    return nullptr;
}

bool GameManager::attemptPlayCardInLobby(int lobbyId, int playerId, size_t handIndex, size_t pileIndex)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) return false;

    Lobby& lobby = it->second;
    if (lobby.getStatus() != LobbyStatus::InProgress) return false;

    Game* game = lobby.getGame();
    if (!game) return false;

    // Validate playerId is current player (to be added later)

    return game->attemptPlayCard(handIndex, pileIndex);
}

bool GameManager::attemptEndTurnInLobby(int lobbyId, int playerId)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) return false;

    Lobby& lobby = it->second;
    if (lobby.getStatus() != LobbyStatus::InProgress) return false;

    Game* game = lobby.getGame();
    if (!game) return false;

    // Validate playerId is current player (to be added later)
    return game->attemptEndTurn();
}