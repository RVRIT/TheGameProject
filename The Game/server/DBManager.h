#pragma once
#include "crow.h"
#include <sqlite3.h>
#include <filesystem>
#include <optional>
class DBManager
{
private:
    sqlite3* db;
public:
    // getGameState needs to be added to return a json which will be used to update game info for users
    bool initialize(const std::string& db_path);
    bool insertGameSession(int user_id, int score);
    bool updateUserStats(int user_id, bool won, double hours_played);
    bool checkExistingUser(const std::string& username);
    std::optional<std::string> getHashedPassword(const std::string& username);
    std::optional<int> getUserId(const std::string& username);
    bool registerUser(const std::string& username, const std::string& hashed_password);
    sqlite3* get_db() { return db; }
    ~DBManager() { if (db) sqlite3_close(db); }
};
