#include "DBManager.h"
int main() {
    crow::SimpleApp app;
    DBManager db;
    if (!db.initialize("./data/game.db")) {
        std::cerr << "ERORR initializing DB!" << std::endl;
        return 1;
    }
    CROW_ROUTE(app, "/")([]() {
        return "Hello, world!";
        });
    app.port(18080).multithreaded().run();
}