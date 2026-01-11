#pragma once
#include <SFML/Network.hpp>
#include <utility>
#include <string>

class NetworkClient {
private:
    std::string host;
    unsigned short port;

public:
    NetworkClient(const std::string& host = "http://localhost", unsigned short port = 18080);
    std::pair<bool, std::string> loginUser(const std::string& username, const std::string& password);
    std::pair<bool, std::string> registerUser(const std::string& username, const std::string& password);
    bool sendStartGameRequest(int lobbyId, int playerId);
    std::string getLobbyStatus();
    void sendLobbyMessage(const std::string& msg);

    int createLobby(const std::string& hostName);
    bool joinLobby(int lobbyId, const std::string& playerName);

    std::string getLobbyState(int lobbyId);
    bool sendLobbyChat(int lobbyId, const std::string& playerName, const std::string& message);

    bool setPlayerReady(int lobbyId, int playerId, bool ready);

    bool startGame(int lobbyId, int playerId);
    std::string getGameState(int lobbyId, const std::string& playerName);
    bool playCard(int lobbyId, const std::string& playerName, int handIndex, int pileIndex);
    bool endTurn(int lobbyId, const std::string& playerName);

    bool kickPlayer(int lobbyId, const std::string& hostName, const std::string& targetName);
};