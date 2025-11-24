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

    Lobby currentLobby;
    int currentLobbyId;
    // Mutex for Thread Safety
    std::mutex m_mtx;

public: 

    // Constructorul la GameManager va fi chemat doar atunci cand se lucreaza cu lobby-ul in sine 
    // pentru a putea da refresh la game state, etc, restul se vor salva in DB

    // Chemi constructorul -> faci actiunea -> chemi destructorul 
    GameManager(int lobbyId) : currentLobbyId{ lobbyId } {};

    // Ne mai gandim la tipul returnat de functii, inca nu s sigur

    // MA VOI OCUPA DE APELURILE DIN DBMANAGER ASTFEL INCAT JOCUL SA FIE UPDATAT DOAR DIN DB
    // PENTRU A NU OCUPA MEMORIE

    int createLobby(const std::string& name, const int lobbyId, const std::string& hostName);

    int joinLobby(const int& lobbyId, const std::string& playerName);

    Lobby& getGameState(const int& lobbyId);
    ~GameManager() = default;
};

