#pragma once
#include <SFML/Network.hpp>
#include <string>

class NetworkClient {
private:
    std::string host;
    unsigned short port;

public:
    NetworkClient(const std::string& host = "http://localhost", unsigned short port = 18080);
    bool loginUser(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password);
};