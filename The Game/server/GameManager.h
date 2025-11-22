#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include "crow.h"

#include "../Game.h"


class GameManager
{
private:
    // Structura interna pentru o camera de asteptare
    struct Lobby {
        std::string hostName;       // numele jucatorului care a creat camera
        std::vector<std::string> players;
        bool isStarted = false;
    };

    // Stocarea datelor
    std::map<std::string, Lobby> m_lobbies;
    std::map<std::string, std::unique_ptr<Game>> m_activeGames;

    // Mutex for Thread Safety
    std::mutex m_mtx;

public: 
    GameManager() = default;

    crow::response createLobby(const std::string& lobbyName, const std::string& hostName);

    crow::response joinLobby(const std::string& lobbyName, const std::string& playerName);

    crow::json::wvalue getGameState(const std::string& lobbyName);


};

