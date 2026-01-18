#define _CRT_SECURE_NO_WARNINGS
#include "DBManager.h"
#include "PasswordService.h"
#include <regex>
#include <crow.h>
#include "GameManager.h"
#include "Lobby.h"
#include <ctime>
#include <cmath>

using namespace crow;

int main() {
    crow::SimpleApp app;
    DBManager& db = DBManager::getInstance();

    if (!db.initialize("./data/game.db")) {
        std::cerr << "ERROR initializing DB!" << std::endl;
        return 1;
    }

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([&db](const crow::request& req) {

        try
        {
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
        }
        catch (const std::exception& e) 
        {
            std::cerr << "[EXCEPTION] /login: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) 
        {
            std::cerr << "[CRITICAL] Unknown error in /login" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });


    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)([&db](const crow::request& req) {
        try
        {

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
            if (username.length() < 3 || username.length() > 20) {
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
            if (password.length() < 6) {
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
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /register: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /register" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/lobby/create").methods("POST"_method)([&db](const crow::request& req) {

        try
        {

            auto body = crow::json::load(req.body);
            if (!body || !body.has("hostName")) {
                return crow::response(400, "Missing 'hostName'");
            }

            std::string hostName = body["hostName"].s();
            float hostRating = db.getUserRating(hostName).value();
            int newLobbyId = GameManager::getInstance().createLobby(hostName, hostRating);

            crow::json::wvalue res;
            res["lobbyId"] = newLobbyId;
            res["message"] = "Lobby created successfully";

            crow::response r(201, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/create: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/create" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/lobby/join").methods("POST"_method)([&db](const crow::request& req) {

        try
        {
            auto body = crow::json::load(req.body);
            if (!body || !body.has("lobbyId") || !body.has("playerName")) {
                return crow::response(400, "Missing 'lobbyId' or 'playerName'");
            }

            int lobbyId = body["lobbyId"].i();
            std::string playerName = body["playerName"].s();
            float playerRating = db.getUserRating(playerName).value();
            bool success = GameManager::getInstance().joinLobby(lobbyId, playerName, playerRating);
            if (success) {
                crow::json::wvalue res;
                res["message"] = "Joined successfully";
                crow::response r(200, res.dump());
                r.set_header("Content-Type", "application/json");
                return r;
            }
            else {
                return crow::response(404, "Lobby full or not found");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/join: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/join" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/chat").methods("POST"_method)([](const crow::request& req, int lobbyId) {

        try
        {
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
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/<int>/chat: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/<int>/chat" << std::endl;
            return crow::response(500, "Critical Error");
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
            crow::response r(200, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        catch (const std::out_of_range& e) {
            return crow::response(404, e.what());
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/state").methods("GET"_method)([](int lobbyId) {

        try
        {
            Lobby* lobby = GameManager::getInstance().getLobby(lobbyId);

            if (lobby) {
                crow::json::wvalue stateJson = lobby->getStateJson();
                crow::response res(200, stateJson.dump());
                res.set_header("Content-Type", "application/json");
                return res;
            }
            else {
                return crow::response(404, "Lobby not found");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/<int>/state: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/<int>/state" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/ready").methods("POST"_method)([](const crow::request& req, int lobbyId) {
        try
        {
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
                crow::response r(200, res.dump());
                r.set_header("Content-Type", "application/json");
                return r;
            }
            else {
                return crow::response(404, "Lobby or player not found");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/<int>/ready: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/<int>/ready" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/start_game").methods("POST"_method)([](const crow::request& req) {

        try
        {
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
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /start_game: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /start_game" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/game/state").methods("GET"_method)([](const crow::request& req, int lobbyId) {

        try
        {
            char* playerNameQuery = req.url_params.get("playerName");
            if (!playerNameQuery) return crow::response(400, "Missing 'playerName' query param");

            std::string playerName = playerNameQuery;
            Lobby* lobby = GameManager::getInstance().getLobby(lobbyId);

            if (!lobby || lobby->getStatus() != LobbyStatus::InProgress) {
                return crow::response(404, "Game not in progress");
            }

            Game* m_game = lobby->getGame();
            if (!m_game) return crow::response(500, "Game instance missing");

            GameSnapshot snap = m_game->getSnapshot(playerName);

            crow::json::wvalue res;
            res["deckSize"] = snap.deckSize;
            res["isGameOver"] = snap.isGameOver;
            res["playerWon"] = snap.playerWon;

            res["cardsPlayedThisTurn"] = snap.cardsPlayedThisTurn;
            res["minCardsToPlay"] = snap.minCardsToPlay;

            for (size_t i = 0; i < snap.piles.size(); ++i) {
                res["piles"][i]["type"] = snap.piles[i].type;
                res["piles"][i]["topValue"] = snap.piles[i].topValue;
            }

            res["myHand"] = snap.myHand;

            for (size_t i = 0; i < snap.players.size(); ++i) {
                res["players"][i]["name"] = snap.players[i].name;
                res["players"][i]["cardCount"] = snap.players[i].cardCount;
                res["players"][i]["isTurn"] = snap.players[i].isCurrentPlayer;
            }

            crow::response r(200, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/<int>/game/state: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/<int>/game/state" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/game/play").methods("POST"_method)([](const crow::request& req, int lobbyId) {
        try
        {
            auto body = crow::json::load(req.body);
            if (!body || !body.has("playerName") || !body.has("handIndex") || !body.has("pileIndex")) {
                return crow::response(400, "Invalid params: playerName, handIndex, pileIndex required");
            }

            std::string playerName = body["playerName"].s();
            size_t handIndex = body["handIndex"].u();
            size_t pileIndex = body["pileIndex"].u();

            bool success = GameManager::getInstance().attemptPlayCardInLobby(lobbyId, playerName, handIndex, pileIndex);

            crow::json::wvalue res;
            if (success) {
                res["status"] = "success";
                crow::response r(200, res.dump());
                r.set_header("Content-Type", "application/json");
                return r;
            }
            else {
                res["status"] = "error";
                res["message"] = "Not your turn or invalid move";
                crow::response r(403, res.dump());
                r.set_header("Content-Type", "application/json");
                return r;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/<int>/game/play: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/<int>/game/play" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/game/end-turn").methods("POST"_method)([](const crow::request& req, int lobbyId) {

        try
        {
            auto body = crow::json::load(req.body);
            if (!body || !body.has("playerName")) {
                return crow::response(400, "Missing 'playerName'");
            }

            std::string playerName = body["playerName"].s();
            bool success = GameManager::getInstance().attemptEndTurnInLobby(lobbyId, playerName);

            crow::json::wvalue res;
            if (success) {
                res["status"] = "success";
                crow::response r(200, res.dump());
                r.set_header("Content-Type", "application/json");
                return r;
            }
            else {
                res["status"] = "error";
                res["message"] = "Cannot end turn (not your turn or rules not met)";
                crow::response r(400, res.dump());
                r.set_header("Content-Type", "application/json");
                return r;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/<int>/game/end-turn: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/<int>/game/end-turn" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/user/stats").methods("GET"_method)([](const crow::request& req) {

        try
        {

            char* usernameParam = req.url_params.get("username");
            if (!usernameParam) return crow::response(400, "Missing username param");

            std::string username = usernameParam;

            auto userOpt = DBManager::getInstance().getUserStats(username);
            if (!userOpt) return crow::response(404, "User not found");

            crow::json::wvalue res;
            res["username"] = userOpt->username;
            res["games_played"] = userOpt->games_played;
            res["games_won"] = userOpt->games_won;
            res["hours_played"] = userOpt->hours_played;
            res["rating"] = userOpt->rating;

            crow::response r(200, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /user/stats: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /user/stats" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/restart").methods("POST"_method)([](const crow::request& req, int lobbyId) {

        try
        {
            auto body = crow::json::load(req.body);
            if (!body || !body.has("playerName")) {
                return crow::response(400, "Missing 'playerName'");
            }

            Lobby* lobby = GameManager::getInstance().getLobby(lobbyId);
            if (!lobby) {
                return crow::response(404, "Lobby not found");
            }

            bool success = GameManager::getInstance().restartGame(lobbyId);

            if (success) {
                crow::json::wvalue res;
                res["status"] = "success";
                res["message"] = "Game reset to lobby state";
                crow::response r(200, res.dump());
                r.set_header("Content-Type", "application/json");
                return r;
            }
            else {
                return crow::response(500, "Could not restart game");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/<int>/restart: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/<int>/restart" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/kick").methods("POST"_method)([](const crow::request& req, int lobbyId) {

        try
        {
            auto body = crow::json::load(req.body);
            if (!body || !body.has("hostName") || !body.has("targetName")) {
                return crow::response(400, "Missing 'hostName' or 'targetName'");
            }

            std::string hostName = body["hostName"].s();
            std::string targetName = body["targetName"].s();

            Lobby* lobby = GameManager::getInstance().getLobby(lobbyId);
            if (!lobby) return crow::response(404, "Lobby not found");

            auto m_players = lobby->getPlayers();
            if (m_players.empty() || m_players[0].name != hostName) {
                return crow::response(403, "Only the host can kick players");
            }

            if (hostName == targetName) {
                return crow::response(400, "Host cannot kick themselves");
            }

            bool success = GameManager::getInstance().removePlayer(lobbyId, targetName);

            if (success) {
                return crow::response(200, "Player kicked successfully");
            }
            else {
                return crow::response(400, "Player not found");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/<int>/kick: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/<int>/kick" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/leave").methods("POST"_method)([](const crow::request& req, int lobbyId) {

        try
        {
            auto body = crow::json::load(req.body);
            if (!body || !body.has("playerName")) {
                return crow::response(400, "Missing 'playerName'");
            }

            std::string playerName = body["playerName"].s();

            Lobby* lobby = GameManager::getInstance().getLobby(lobbyId);
            if (!lobby) return crow::response(404, "Lobby not found");

            bool success = GameManager::getInstance().removePlayer(lobbyId, playerName);

            if (success) {
                return crow::response(200, "Left lobby successfully");
            }
            else {
                return crow::response(400, "Player not found");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/<int>/leave: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/<int>/leave" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    
    CROW_ROUTE(app, "/lobby/list").methods("GET"_method)([]() {

        try
        {

            auto summaries = GameManager::getInstance().listLobbies();

            crow::json::wvalue res = crow::json::wvalue::list();
            for (size_t i = 0; i < summaries.size(); ++i) {
                res[i]["id"] = summaries[i].id;
                res[i]["hostName"] = summaries[i].hostName;
                res[i]["playersCount"] = summaries[i].playersCount;
                res[i]["maxPlayers"] = summaries[i].maxPlayers;
                res[i]["status"] = summaries[i].status;
                res[i]["avgRating"] = summaries[i].avgRating;
            }

            crow::response r(200, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/list: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/list" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>").methods("DELETE"_method)([](int lobbyId) {

        try
        {

            bool ok = GameManager::getInstance().deleteLobby(lobbyId);

            crow::json::wvalue res;
            if (!ok) {
                res["status"] = "error";
                res["message"] = "Lobby not found";
                crow::response r(404, res.dump());
                r.set_header("Content-Type", "application/json");
                return r;
            }

            res["status"] = "success";
            res["message"] = "Lobby deleted";
            crow::response r(200, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/<int>: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/<int>" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    CROW_ROUTE(app, "/lobby/<int>/finish").methods("POST"_method)
        ([&](const crow::request& req, int lobbyId) {

        try
        {

            auto body = crow::json::load(req.body);
            if (!body || !body.has("winnerUsername")) {
                return crow::response(400, "Missing 'winnerUsername'");
            }

            std::string winner = body["winnerUsername"].s();

            double durationHours = 0.0;
            if (body.has("durationHours")) {
                durationHours = body["durationHours"].d();
                if (durationHours < 0.0) durationHours = 0.0;
            }

            bool ok = GameManager::getInstance().saveGameResults(lobbyId, winner, durationHours);
            if (!ok) {
                crow::json::wvalue res;
                res["status"] = "error";
                res["message"] = "Cannot finish match";
                crow::response r(400, res.dump());
                r.set_header("Content-Type", "application/json");
                return r;
            }

            crow::json::wvalue res;
            res["status"] = "success";
            res["message"] = "Match finished, stats saved, lobby removed";
            crow::response r(200, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/<int>/finish: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/<int>/finish" << std::endl;
            return crow::response(500, "Critical Error");
        }

            });

    CROW_ROUTE(app, "/lobby/quickplay").methods("POST"_method)([&db](const crow::request& req) {

        try
        {
            auto body = crow::json::load(req.body);
            if (!body || !body.has("playerName")) {
                return crow::response(400, "Missing 'playerName'");
            }

            std::string playerName = body["playerName"].s();

            auto ratingOpt = db.getUserRating(playerName);
            if (!ratingOpt) {
                return crow::response(404, "User not found");
            }
            float playerRating = *ratingOpt;

            auto& gm = GameManager::getInstance();
            auto result = gm.quickplay(playerName, playerRating);

            crow::json::wvalue res;
            res["lobbyId"] = result.lobbyId;
            res["created"] = result.created;

            int bucketLow = static_cast<int>(std::floor(playerRating));
            res["bucketLow"] = bucketLow;
            res["bucketHigh"] = bucketLow + 1;

            res["playerRating"] = playerRating;

            auto avgOpt = gm.getLobbyAverageRating(result.lobbyId);
            if (avgOpt) res["lobbyAvgRating"] = *avgOpt;

            crow::response r(result.created ? 201 : 200, res.dump());
            r.set_header("Content-Type", "application/json");
            return r;
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] /lobby/quickplay: " << e.what() << std::endl;
            return crow::response(500, "Internal Server Error");
        }
        catch (...) {
            std::cerr << "[CRITICAL] Unknown error in /lobby/quickplay" << std::endl;
            return crow::response(500, "Critical Error");
        }
        });

    app.port(18080).multithreaded().run();
}
