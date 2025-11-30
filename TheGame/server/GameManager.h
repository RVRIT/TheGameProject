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

    bool createLobby(int lobbyId, const std::string& hostName);
    bool joinLobby(int lobbyId, const std::string& playerName);
    Lobby& getLobby(int lobbyId); // redenumit pentru claritate

private:
    GameManager() = default;
    std::map<int, Lobby> m_lobbies;
    mutable std::mutex m_mtx;
};