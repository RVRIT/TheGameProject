#include "DBManager.h"
#include "PasswordService.h"
#include <regex>
#include <crow.h>
#include "GameManager.h"
#include "Lobby.h"

using namespace crow;

int main() {
    crow::SimpleApp app;
    DBManager& db = DBManager::getInstance();

    if (!db.initialize("./data/game.db")) {
        std::cerr << "ERROR initializing DB!" << std::endl;
        return 1;
    }

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([&db](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "JSON invalid: username and password required!");
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();
        crow::json::wvalue res;

        auto stored_hash_opt = db.getHashedPassword(username);
        if (!stored_hash_opt) {
            res["status"] = "error";
            res["message"] = "Invalid username or password";
            crow::response r(401, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }

        const std::string& stored_hash = *stored_hash_opt;
        if (PasswordService::verifyPassword(password, stored_hash)) {
            res["status"] = "success";
            res["message"] = "Login successful";
            crow::response r(200, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        else {
            res["status"] = "error";
            res["message"] = "Invalid username or password";
            crow::response r(401, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        });

    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)([&db](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "JSON invalid: username and password required!");
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();
        crow::json::wvalue res;
        std::regex validUsernamePattern("^[a-zA-Z0-9]+$");
        if (!std::regex_match(username, validUsernamePattern)) {
            res["status"] = "error";
            res["message"] = "Username must contain only letters and numbers";
            crow::response r(400, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        if (username.length() < 3 || username.length() > 20)
        {
            res["status"] = "error";
            res["message"] = "Username must be between 3-20 characters long";
            crow::response r(400, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        if (db.checkExistingUser(username)) {
            res["status"] = "error";
            res["message"] = "Username taken";
            crow::response r(409, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        if (password.length() < 6)
        {
            res["status"] = "error";
            res["message"] = "Password must be at least 6 characters long";
            crow::response r(400, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        std::string hashed = PasswordService::hashPassword(password);
        if (db.registerUser(username, hashed)) {
            res["status"] = "success";
            res["message"] = "Registered successfully";
            crow::response r(200, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        else {
            res["status"] = "error";
            res["message"] = "Error while registering, please try again";
            crow::response r(500, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        });

    CROW_ROUTE(app, "/lobby/create").methods("POST"_method)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("hostName")) {
            return crow::response(400, "Missing 'hostName'");
        }

        std::string hostName = body["hostName"].s();
        int newLobbyId = GameManager::getInstance().createLobby(hostName);

        crow::json::wvalue res;
        res["lobbyId"] = newLobbyId;
        res["message"] = "Lobby created successfully";

        return crow::response(201, res.dump());
        });

    CROW_ROUTE(app, "/lobby/join").methods("POST"_method)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("lobbyId") || !body.has("playerName")) {
            return crow::response(400, "Missing 'lobbyId' or 'playerName'");
        }

        int lobbyId = body["lobbyId"].i();
        std::string playerName = body["playerName"].s();

        bool success = GameManager::getInstance().joinLobby(lobbyId, playerName);
        if (success) {
            crow::json::wvalue res;
            res["message"] = "Joined successfully";
            return crow::response(200, res.dump());
        }
        else {
            return crow::response(404, "Lobby full or not found");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/chat").methods("POST"_method)([](const crow::request& req, int lobbyId) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("sender") || !body.has("content")) {
            return crow::response(400, "Missing 'sender' or 'content'");
        }

        std::string sender = body["sender"].s();
        std::string content = body["content"].s();

        bool success = GameManager::getInstance().sendChatMessage(lobbyId, sender, content);
        if (success) {
            return crow::response(200, "Message sent");
        }
        else {
            return crow::response(404, "Lobby not found or invalid message");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/chat")([](int lobbyId) {
        try {
            auto messages = GameManager::getInstance().getChatHistory(lobbyId);

            std::vector<crow::json::wvalue> jsonMessages;
            for (const auto& msg : messages) {
                crow::json::wvalue m;
                m["sender"] = msg.sender;
                m["content"] = msg.content;
                jsonMessages.push_back(std::move(m));
            }

            crow::json::wvalue res = crow::json::wvalue(jsonMessages);
            return crow::response(200, res.dump());
        }
        catch (const std::out_of_range& e) {
            return crow::response(404, e.what());
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/state").methods("GET"_method)([](int lobbyId) {
        Lobby* lobby = GameManager::getInstance().getLobby(lobbyId);

        if (lobby) {
            crow::json::wvalue stateJson = lobby->getStateJSON();
            crow::response res(200, stateJson.dump());
            res.set_header("Content-Type", "application/json");
            return res;
        }
        else {
            return crow::response(404, "Lobby not found");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/ready").methods("POST"_method)([](const crow::request& req, int lobbyId) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("playerId") || !body.has("ready")) {
            return crow::response(400, "Missing 'playerId' or 'ready'");
        }

        int playerId = body["playerId"].i();
        bool ready = body["ready"].b();

        auto& gameManager = GameManager::getInstance();
        bool success = gameManager.setPlayerReady(lobbyId, playerId, ready);

        if (success) {
            crow::json::wvalue res;
            res["message"] = "Updated status";
            return crow::response(200, res.dump());
        }
        else {
            return crow::response(404, "Lobby or player not found");
        }
        });

    CROW_ROUTE(app, "/start_game").methods("POST"_method)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("lobbyId") || !body.has("playerId")) {
            return crow::response(400, "Missing 'lobbyId' or 'playerId'");
        }

        int lobbyId = body["lobbyId"].i();
        int playerId = body["playerId"].i();

        bool success = GameManager::getInstance().attemptStartGame(lobbyId, playerId);

        if (success) {
            return crow::response(200, "Game Started");
        }
        else {
            return crow::response(400, "Cannot start game");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/game/end-turn").methods("POST"_method)([](const crow::request& req, int lobbyId) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("playerId")) {
            return crow::response(400, "Missing 'playerId'");
        }

        int playerId = body["playerId"].i();

        auto& gameManager = GameManager::getInstance();
        bool success = gameManager.attemptEndTurnInLobby(lobbyId, playerId);

        crow::json::wvalue res;
        if (success) {
            res["status"] = "success";
            res["message"] = "Turn ended";
            return crow::response(200, res.dump());
        }
        else {
            res["status"] = "error";
            res["message"] = "Cannot end turn (too few cards played, not your turn, or game over)";
            return crow::response(400, res.dump());
        }
        });

    app.port(18080).multithreaded().run();
}