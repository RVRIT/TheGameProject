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

bool GameManager::leaveLobby(int lobbyId, const std::string& playerName)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) return false;

    
     //  it->second.removePlayerByName(playerName);   //needs to be implemented in Lobby

    if (it->second.getPlayers().empty()) {
        m_lobbies.erase(it);
    }
    return true;
}

bool GameManager::attemptPlayCardInLobby(int lobbyId, const std::string& playerName, size_t handIndex, size_t pileIndex) {
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) return false;

    Lobby& lobby = it->second;
    if (lobby.getStatus() != LobbyStatus::InProgress) return false;

    Game* m_game = lobby.getGame();
    if (!m_game) return false;

    if (!m_game->isPlayerTurn(playerName)) {
        return false;
    }

    return m_game->attemptPlayCard(handIndex, pileIndex);
}

bool GameManager::attemptEndTurnInLobby(int lobbyId, const std::string& playerName) {
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) return false;

    Lobby& lobby = it->second;
    Game* m_game = lobby.getGame();
    if (!m_game || lobby.getStatus() != LobbyStatus::InProgress) return false;

    if (!m_game->isPlayerTurn(playerName)) {
        return false;
    }

    return m_game->attemptEndTurn();
}
bool GameManager::attemptStartGame(int lobbyId, int requestPlayerId)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);

    if (it == m_lobbies.end()) {
        return false;
    }

    return it->second.tryStartGame(requestPlayerId);
}

bool GameManager::restartGame(int lobbyId) {
    Lobby* lobby = getLobby(lobbyId);
    if (lobby) {
        lobby->resetGame();
        return true;
    }
    return false;
}

bool GameManager::kickPlayer(int lobbyId, const std::string& playerName) {
    Lobby* lobby = getLobby(lobbyId);
    if (!lobby) return false;

    int playerIdToRemove = -1;
    for (const auto& p : lobby->getPlayers()) {
        if (p.name == playerName) {
            playerIdToRemove = p.id;
            break;
        }
    }

    if (playerIdToRemove != -1) {
        lobby->removePlayer(playerIdToRemove);
        return true;
    }
    return false;
}