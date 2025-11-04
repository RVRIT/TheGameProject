#include "DBManager.h"
int main() {
    crow::SimpleApp app;
    DBManager db;
    if (!db.initialize("./data/game.db")) {
        std::cerr << "ERORR initializing DB!" << std::endl;
        return 1;
    }
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([&db](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username")) {
            return crow::response(400, "JSON invalid: username missing!");
        }

        std::string username = body["username"].s();
        crow::json::wvalue res;

        if (db.checkExistingUser(username)) {
            res["status"] = "success";
            res["message"] = "Login successful";

            crow::response r;
            r.code = 200;
            r.body = res.dump();
            r.set_header("Content-Type", "application/json");
            return r;
        }
        else {
            res["status"] = "error";
            res["message"] = "User not found, please register.";

            crow::response r;
            r.code = 401;
            r.body = res.dump();
            r.set_header("Content-Type", "application/json");
            return r;
        }
        });
    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)([&db](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username")) {
            return crow::response(400, "JSON invalid: username missing!");
        }

        std::string username = body["username"].s();
        crow::json::wvalue res;

        if (db.checkExistingUser(username)) {
            res["status"] = "error";
            res["message"] = "Username taken";

            crow::response r;
            r.code = 409;
            std::string body_str = res.dump();
            r.body = body_str;
            r.set_header("Content-Type", "application/json");
            return r;
        }

        if (db.registerUser(username)) {
            res["status"] = "success";
            res["message"] = "Registered successfully";

            crow::response r;
            r.code = 200;
            std::string body_str = res.dump();
            r.body = body_str;
            r.set_header("Content-Type", "application/json");
            return r;
        }
        else {
            res["status"] = "error";
            res["message"] = "Error while registering, please try again";

            crow::response r;
            r.code = 500;
            std::string body_str = res.dump();
            r.body = body_str;
            r.set_header("Content-Type", "application/json");
            return r;
        }
        });
    app.port(18080).multithreaded().run();
}