#include "NetworkClient.h"
#include <iostream>

NetworkClient::NetworkClient(const std::string& host, unsigned short port)
    : host(host), port(port) {}

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
        return { false, response.getBody()};
    
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
