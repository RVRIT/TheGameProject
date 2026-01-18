#pragma once
#include <map>
#include <string>
#include <mutex>
#include <vector>
#include <optional>
#include "Lobby.h"
class GameManager
{
public:
    static GameManager& getInstance();

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    int createLobby(const std::string& hostName, float hostRating);
    bool joinLobby(int lobbyId, const std::string& playerName, float playerRating);
    bool sendChatMessage(int lobbyId, const std::string& sender, const std::string& content);
    std::vector<ChatMessage> getChatHistory(int lobbyId) const;
    bool setPlayerReady(int lobbyId, int playerId, bool ready);
    bool attemptPlayCardInLobby(int lobbyId, const std::string& playerName, size_t handIndex, size_t pileIndex);
    bool attemptEndTurnInLobby(int lobbyId, const std::string& playerName);
    bool attemptStartGame(int lobbyId, int requestPlayerId);
    Lobby* getLobby(int lobbyId);
    bool leaveLobby(int lobbyId, const std::string& playerName);
    bool restartGame(int lobbyId);
    bool removePlayer(int lobbyId, const std::string& playerName);
    bool saveGameResults(int lobbyId, const std::string& winnerUsername, double durationHours);
    struct LobbySummary {
        int id;
        std::string hostName;
        int playersCount;
        int maxPlayers;
        float avgRating;
        std::string status; 
    };

    std::vector<LobbySummary> listLobbies() const;
    bool deleteLobby(int lobbyId);
    std::vector<std::string> getLobbyPlayerNames(int lobbyId) const;
    std::optional<float> getLobbyAverageRating(int lobbyId) const;

    struct QuickplayResult {
        int lobbyId;
        bool created;
    };

    QuickplayResult quickplay(const std::string& playerName, float playerRating);

private:
    GameManager() = default;
    std::map<int, Lobby> m_lobbies;
    mutable std::mutex m_mtx;
    int m_nextLobbyId = 1;
};