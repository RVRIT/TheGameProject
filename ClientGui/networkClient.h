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
};