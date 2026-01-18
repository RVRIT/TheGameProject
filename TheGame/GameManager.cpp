#include "GameManager.h"
#include "DBManager.h"
#include "GameSnapshot.h"
#include <stdexcept>
#include <cmath>
#include <limits>

GameManager& GameManager::getInstance()
{
    static GameManager instance;
    return instance;
}

int GameManager::createLobby(const std::string& hostName, float hostRating)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    int newId = m_nextLobbyId++;
    m_lobbies.try_emplace(newId, newId, hostName, hostRating);
    return newId;
}

bool GameManager::joinLobby(int lobbyId, const std::string& playerName, float playerRating)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end())
        return false;
    if (it->second.isPlayerInLobby(playerName))
    {
        return false;
    }
    return it->second.addPlayer(playerName, playerRating);
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

bool GameManager::removePlayer(int lobbyId, const std::string& playerName) {
    std::lock_guard<std::mutex> lock(m_mtx);

    auto it = m_lobbies.find(lobbyId); 
    if (it == m_lobbies.end()) return false;

    Lobby& lobby = it->second;

    int playerIdToRemove = -1;
    for (const auto& p : lobby.getPlayers()) {
        if (p.name == playerName) {
            playerIdToRemove = p.id;
            break;
        }
    }

    if (playerIdToRemove != -1) 
    {
        lobby.removePlayer(playerIdToRemove);
        if (lobby.getPlayers().empty())
        {
            m_lobbies.erase(it);        // if lobby is empty , we remove it
        }
        return true;
    }
    return false;
}


std::vector<GameManager::LobbySummary> GameManager::listLobbies() const
{
    std::lock_guard<std::mutex> lock(m_mtx);

    std::vector<LobbySummary> out;
    out.reserve(m_lobbies.size());

    for (const auto& [id, lobby] : m_lobbies) {
        LobbySummary s;
        s.id = id;

        const auto& players = lobby.getPlayers();
        s.playersCount = static_cast<int>(players.size());
        s.hostName = players.empty() ? "" : players[0].name;
        s.avgRating = players.empty() ? 0.0 : lobby.getAverageRating();
        s.maxPlayers = 5;

        auto st = lobby.getStatus();
        if (st == LobbyStatus::Waiting) s.status = "Waiting";
        else if (st == LobbyStatus::InProgress) s.status = "InProgress";
        else s.status = "Finished";

        out.push_back(std::move(s));
    }

    return out;
}

bool GameManager::deleteLobby(int lobbyId)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    return m_lobbies.erase(lobbyId) > 0;
}

std::vector<std::string> GameManager::getLobbyPlayerNames(int lobbyId) const
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) return {};

    std::vector<std::string> names;
    for (const auto& p : it->second.getPlayers()) {
        names.push_back(p.name);
    }
    return names;
}

bool GameManager::saveGameResults(int lobbyId, const std::string& winnerUsername, double durationHours)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end())
        return false;

    Lobby& lobby = it->second;

    Game* game = lobby.getGame();
    if (!game)
        return false;

    DBManager& db = DBManager::getInstance();

    int updatedUsers = 0;

    for (const auto& p : lobby.getPlayers())
    {
        auto userIdOpt = db.getUserId(p.name);
        if (!userIdOpt)
            continue;

        bool won = (p.name == winnerUsername);

        GameSnapshot snap = game->getSnapshot(p.name);
        int cardsLeftInHand = static_cast<int>(snap.myHand.size());

        db.updateUserStats(*userIdOpt, won, durationHours, cardsLeftInHand);

        int score = 0;
        db.insertGameSession(*userIdOpt, score, durationHours, won ? "win" : "loss");

        updatedUsers++;
    }

    m_lobbies.erase(it);

    return updatedUsers > 0;
}

std::optional<float> GameManager::getLobbyAverageRating(int lobbyId) const
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto it = m_lobbies.find(lobbyId);
    if (it == m_lobbies.end()) return std::nullopt;
    return it->second.getAverageRating();
}

GameManager::QuickplayResult GameManager::quickplay(const std::string& playerName, float playerRating)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    const int bucket = static_cast<int>(std::floor(playerRating));

    int bestLobbyId = -1;
    float bestScore = std::numeric_limits<float>::max();

    for (auto& [id, lobby] : m_lobbies)
    {
        if (lobby.isPlayerInLobby(playerName))
            continue;

        if (lobby.getStatus() != LobbyStatus::Waiting)
            continue;

        if (lobby.getPlayers().size() >= 5)
            continue;

        const float avg = lobby.getAverageRating();
        if (static_cast<int>(std::floor(avg)) != bucket)
            continue;

        const float score = std::abs(avg - playerRating);
        if (score < bestScore)
        {
            bestScore = score;
            bestLobbyId = id;
        }
    }

    if (bestLobbyId != -1)
    {
        Lobby& lobby = m_lobbies.at(bestLobbyId);
        if (lobby.addPlayer(playerName, playerRating))
        {
            return { bestLobbyId, false };
        }
    }

    int newId = m_nextLobbyId++;
    m_lobbies.try_emplace(newId, newId, playerName, playerRating);
    return { newId, true };
}


