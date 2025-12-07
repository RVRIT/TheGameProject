#pragma once
#include <map>
#include <string>
#include <mutex>
#include "../Lobby.h"

class GameManager
{
public:
    static GameManager& getInstance();

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    int createLobby(const std::string& hostName);
    bool joinLobby(int lobbyId, const std::string& playerName);

private:
    GameManager() = default;
    std::map<int, Lobby> m_lobbies;
    mutable std::mutex m_mtx;
    int m_nextLobbyId = 1;
};