#include "NetworkClient.h"
#include <iostream>

NetworkClient::NetworkClient(const std::string& host, unsigned short port)
    : host(host), port(port) {}

bool NetworkClient::loginUser(const std::string& username) {
    sf::Http http(host, port);

    sf::Http::Request request;
    request.setMethod(sf::Http::Request::Post);
    request.setUri("/login");
    request.setField("Content-Type", "application/json");
    request.setBody("{\"username\":\"" + username + "\"}");

    sf::Http::Response response = http.sendRequest(request);

    std::cout << "Login response: " << response.getStatus() << " - " << response.getBody() << "\n";

    return response.getStatus() == sf::Http::Response::Ok;
}

bool NetworkClient::registerUser(const std::string& username) {
    sf::Http http(host, port);

    sf::Http::Request request;
    request.setMethod(sf::Http::Request::Post);
    request.setUri("/register");
    request.setField("Content-Type", "application/json");
    request.setBody("{\"username\":\"" + username + "\"}");

    sf::Http::Response response = http.sendRequest(request);

    std::cout << "Login response: " << response.getStatus() << " - " << response.getBody() << "\n";

    return response.getStatus() == sf::Http::Response::Ok;
}