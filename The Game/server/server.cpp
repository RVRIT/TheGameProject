#include "DBManager.h"
#include "PasswordService.h"
#include <regex>
int main() {
    crow::SimpleApp app;
    DBManager db;
    if (!db.initialize("./data/game.db")) {
        std::cerr << "ERORR initializing DB!" << std::endl;
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
            crow::json::wvalue res;
            res["status"] = "error";
            res["message"] = "Username must contain only letters and numbers";
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
        std::string hashed = PasswordService::hashPassword(password);
        std::cout << hashed << std::endl;
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
    app.port(18080).multithreaded().run();
}