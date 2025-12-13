#pragma once
#include <map>
#include <string>
#include <mutex>
#include <vector>
#include "Lobby.h"
class GameManager
{
public:
    static GameManager& getInstance();

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    int createLobby(const std::string& hostName);
    bool joinLobby(int lobbyId, const std::string& playerName);
    bool sendChatMessage(int lobbyId, const std::string& sender, const std::string& content);
    std::vector<ChatMessage> getChatHistory(int lobbyId) const;
    bool setPlayerReady(int lobbyId, int playerId, bool ready);
    bool attemptPlayCardInLobby(int lobbyId, int playerId, size_t handIndex, size_t pileIndex);
    bool attemptEndTurnInLobby(int lobbyId, int playerId);
    //to be fixed soon
    Lobby* getLobby(int lobbyId);

private:
    GameManager() = default;
    std::map<int, Lobby> m_lobbies;
    mutable std::mutex m_mtx;
    int m_nextLobbyId = 1;
};