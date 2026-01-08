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

std::string NetworkClient::getLobbyState(int lobbyId) {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setMethod(sf::Http::Request::Get);
    request.setUri("/lobby/" + std::to_string(lobbyId) + "/state");
    sf::Http::Response response = http.sendRequest(request);

    if (response.getStatus() == sf::Http::Response::Ok) {
        return response.getBody();
    }
    return "";
}

bool NetworkClient::sendLobbyChat(int lobbyId, const std::string& playerName, const std::string& msg) {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setMethod(sf::Http::Request::Post);
    request.setUri("/lobby/" + std::to_string(lobbyId) + "/chat");
    request.setField("Content-Type", "application/json");

    json payload;
    payload["lobbyId"] = lobbyId;
    payload["sender"] = playerName;
    payload["content"] = msg;

    request.setBody(payload.dump());

    sf::Http::Response response = http.sendRequest(request);
    return (response.getStatus() == sf::Http::Response::Ok);
}

int NetworkClient::createLobby(const std::string& hostName) {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setMethod(sf::Http::Request::Post);
    request.setUri("/lobby/create");
    request.setField("Content-Type", "application/json");

    json payload;
    payload["hostName"] = hostName;
    request.setBody(payload.dump());

    sf::Http::Response response = http.sendRequest(request);

    if (response.getStatus() == sf::Http::Response::Created || response.getStatus() == sf::Http::Response::Ok) {
        try {
            auto jsonResponse = json::parse(response.getBody());
            if (jsonResponse.contains("lobbyId")) {
                return jsonResponse["lobbyId"];
            }
        }
        catch (...) {
            std::cerr << "Error parsing lobby creation response\n";
        }
    }
    return -1; 
}

bool NetworkClient::joinLobby(int lobbyId, const std::string& playerName) {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setMethod(sf::Http::Request::Post);
    request.setUri("/lobby/join");
    request.setField("Content-Type", "application/json");

    json payload;
    payload["lobbyId"] = lobbyId;
    payload["playerName"] = playerName;
    request.setBody(payload.dump());

    sf::Http::Response response = http.sendRequest(request);

    return (response.getStatus() == sf::Http::Response::Ok);
}

bool NetworkClient::setPlayerReady(int lobbyId, int playerId, bool ready) {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setUri("/lobby/" + std::to_string(lobbyId) + "/ready");
    request.setMethod(sf::Http::Request::Post);
    request.setField("Content-Type", "application/json");

    json payload;
    payload["playerId"] = playerId;
    payload["ready"] = ready;

    request.setBody(payload.dump());

    sf::Http::Response response = http.sendRequest(request);
    return (response.getStatus() == sf::Http::Response::Ok);
}