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


bool NetworkClient::startGame(int lobbyId, int playerId) {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setMethod(sf::Http::Request::Post);
    request.setUri("/start_game");
    request.setField("Content-Type", "application/json");

    json payload;
    payload["lobbyId"] = lobbyId;
    payload["playerId"] = playerId;
    request.setBody(payload.dump());

    sf::Http::Response response = http.sendRequest(request);
    return (response.getStatus() == sf::Http::Response::Ok);
}

std::string NetworkClient::getGameState(int lobbyId, const std::string& playerName) {
    sf::Http http(host, port);
    sf::Http::Request request;

    std::string uri = "/lobby/" + std::to_string(lobbyId) + "/game/state?playerName=" + playerName;

    request.setMethod(sf::Http::Request::Get);
    request.setUri(uri);

    sf::Http::Response response = http.sendRequest(request);
    if (response.getStatus() == sf::Http::Response::Ok) {
        return response.getBody();
    }
    return "";
}

bool NetworkClient::playCard(int lobbyId, const std::string& playerName, int handIndex, int pileIndex) {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setMethod(sf::Http::Request::Post);
    request.setUri("/lobby/" + std::to_string(lobbyId) + "/game/play");
    request.setField("Content-Type", "application/json");

    json payload;
    payload["playerName"] = playerName;
    payload["handIndex"] = handIndex;
    payload["pileIndex"] = pileIndex;
    request.setBody(payload.dump());

    sf::Http::Response response = http.sendRequest(request);
    return (response.getStatus() == sf::Http::Response::Ok);
}

bool NetworkClient::endTurn(int lobbyId, const std::string& playerName) {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setMethod(sf::Http::Request::Post);
    request.setUri("/lobby/" + std::to_string(lobbyId) + "/game/end-turn");
    request.setField("Content-Type", "application/json");

    json payload;
    payload["playerName"] = playerName;
    request.setBody(payload.dump());

    sf::Http::Response response = http.sendRequest(request);
    return (response.getStatus() == sf::Http::Response::Ok);
}

bool NetworkClient::kickPlayer(int lobbyId, const std::string& hostName, const std::string& targetName) {
    sf::Http http(host, port);

    json j;
    j["hostName"] = hostName;
    j["targetName"] = targetName; // Acum trimitem string-ul corect

    std::string url = "/lobby/" + std::to_string(lobbyId) + "/kick";

    sf::Http::Request req(url, sf::Http::Request::Post);
    req.setBody(j.dump());
    req.setField("Content-Type", "application/json");

    sf::Http::Response response = http.sendRequest(req);

    if (response.getStatus() == sf::Http::Response::Ok) {
        std::cout << "[CLIENT] Kick successful.\n";
        return true;
    }
    else {
        std::cout << "[CLIENT] Kick failed: " << response.getBody() << "\n";
        return false;
    }
}

std::string NetworkClient::getLobbyList() {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setMethod(sf::Http::Request::Get);
    request.setUri("/lobby/list");

    sf::Http::Response response = http.sendRequest(request);

    if (response.getStatus() == sf::Http::Response::Ok) {
        return response.getBody();
    }

    return "[]";
}

std::string NetworkClient::getUserStats(const std::string& username) {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setMethod(sf::Http::Request::Get);
    request.setUri("/user/stats?username=" + username);

    sf::Http::Response response = http.sendRequest(request);

    if (response.getStatus() == sf::Http::Response::Ok) {
        return response.getBody();
    }
    return "{}"; 
}

bool NetworkClient::leaveLobby(int lobbyId, const std::string& playerName) {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setMethod(sf::Http::Request::Post);
    request.setUri("/lobby/" + std::to_string(lobbyId) + "/leave");
    request.setField("Content-Type", "application/json");

    json payload;
    payload["playerName"] = playerName;
    request.setBody(payload.dump());

    sf::Http::Response response = http.sendRequest(request);
    return (response.getStatus() == sf::Http::Response::Ok);
}

bool NetworkClient::deleteLobby(int lobbyId) {
    sf::Http http(host, port);
    sf::Http::Request request;

    request.setMethod(sf::Http::Request::Delete);
    request.setUri("/lobby/" + std::to_string(lobbyId));

    sf::Http::Response response = http.sendRequest(request);

    return (response.getStatus() == sf::Http::Response::Ok);
}