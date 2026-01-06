#include "NetworkClient.h"
#include <iostream>
#include <string>
#include "json.hpp"
using json = nlohmann::json;

NetworkClient::NetworkClient(const std::string& host, unsigned short port)
    : host(host), port(port) {
}

std::pair<bool, std::string> NetworkClient::loginUser(const std::string& username, const std::string& password) {
    sf::Http http(host, port);

    sf::Http::Request request;
    request.setMethod(sf::Http::Request::Post);
    request.setUri("/login");
    request.setField("Content-Type", "application/json");

    std::string body = "{ \"username\": \"" + username + "\", \"password\": \"" + password + "\" }";
    request.setBody(body);

    sf::Http::Response response = http.sendRequest(request);

    std::cout << "Login response: " << response.getStatus()
        << " - " << response.getBody() << "\n";

    if (response.getStatus() == sf::Http::Response::Ok || response.getStatus() == sf::Http::Response::Created)
        return { true, response.getBody() };
    else
        return { false, response.getBody() };
}

std::pair<bool, std::string> NetworkClient::registerUser(const std::string& username, const std::string& password) {
    sf::Http http(host, port);

    sf::Http::Request request;
    request.setMethod(sf::Http::Request::Post);
    request.setUri("/register");
    request.setField("Content-Type", "application/json");

    std::string body = "{ \"username\": \"" + username + "\", \"password\": \"" + password + "\" }";
    request.setBody(body);

    sf::Http::Response response = http.sendRequest(request);

    std::cout << "Register response: " << response.getStatus()
        << " - " << response.getBody() << "\n";

    if (response.getStatus() == sf::Http::Response::Ok || response.getStatus() == sf::Http::Response::Created)
        return { true, response.getBody() };
    else
        return { false, response.getBody() };
}

bool NetworkClient::sendStartGameRequest(int lobbyId, int playerId) {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setMethod(sf::Http::Request::Post);
    request.setUri("/start_game");
    request.setField("Content-Type", "application/json");

    std::string body = "{ \"lobbyId\": " + std::to_string(lobbyId) +
        ", \"playerId\": " + std::to_string(playerId) + " }";
    request.setBody(body);

    sf::Http::Response response = http.sendRequest(request);

    if (response.getStatus() == sf::Http::Response::Ok) {
        std::cout << "Game start request sent successfully.\n";
        return true;
    }
    else {
        std::cout << "Failed to start game: " << response.getBody() << "\n";
        return false;
    }
}

std::string NetworkClient::getLobbyStatus() {
    // MOCK DATA
    return R"({
        "status": "Waiting",
        "MAX_PLAYERS": 5,
        "players": [
            { "id": 0, "name": "RVR", "isReady": true },
            { "id": 1, "name": "PlayerTwo", "isReady": false }
        ],
        "chat": [
            { "sender": "RVR", "content": "Hello!" },
            { "sender": "PlayerTwo", "content": "Ready?" }
        ]
    })";
}

void NetworkClient::sendLobbyMessage(const std::string& msg) {
    // Mock send
}

int NetworkClient::createLobby(const std::string& hostName) {
    sf::Http http(host, port);
    sf::Http::Request request;

    // 1. Prepare the request
    request.setMethod(sf::Http::Request::Post);
    request.setUri("/lobby/create"); // The endpoint the server listens to
    request.setField("Content-Type", "application/json");

    // 2. Create JSON Payload
    json payload;
    payload["hostName"] = hostName;
    request.setBody(payload.dump());

    // 3. Send and Wait for Response
    sf::Http::Response response = http.sendRequest(request);

    // 4. Check if it worked
    if (response.getStatus() == sf::Http::Response::Ok ||
        response.getStatus() == sf::Http::Response::Created) {

        // Parse the response to get the Lobby ID
        auto jsonResponse = json::parse(response.getBody());
        if (jsonResponse.contains("lobbyId")) {
            return jsonResponse["lobbyId"];
        }
    }

    return -1; // Return -1 if it failed
}