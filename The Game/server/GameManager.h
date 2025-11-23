#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include "../Lobby.h"
class GameManager
{
private:

    // se va sterge asta cu map ul de m_lobbies
    // lobby-urile vor fi salvate in DB
    std::map<std::string, Lobby> m_lobbies;

    // Mutex for Thread Safety
    std::mutex m_mtx;

public: 

    GameManager() = default;

    // Ne mai gandim la tipul returnat de functii, inca nu s sigur

    // MA VOI OCUPA DE APELURILE DIN DBMANAGER ASTFEL INCAT JOCUL SA FIE UPDATAT DOAR DIN DB
    // PENTRU A NU OCUPA MEMORIE
    int createLobby(const std::string& name, const int lobbyId, const std::string& hostName);

    int joinLobby(const int& lobbyId, const std::string& playerName);

    Lobby& getGameState(const int& lobbyId);

};

