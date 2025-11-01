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
        if (db.checkLogin(username))
        {
            res["status"] = "success";
            res["message"] = "Login successful";
            return crow::response{ res };
        }
        else {
            res["status"] = "error";
            res["message"] = "User not found, please register.";
            return crow::response{ 401, res };
        }
        });
    app.port(18080).multithreaded().run();
}