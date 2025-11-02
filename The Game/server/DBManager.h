#pragma once
#include "crow.h"
#include <sqlite3.h>
#include <filesystem>
class DBManager
{
private:
    sqlite3* db;
public:
    bool initialize(const std::string& db_path);
    bool insertUser(const std::string& username);
    bool insertGameSession(int user_id, int score);
    bool updateUserStats(int user_id, bool won, double hours_played);
    bool checkLogin(const std::string& username);
    sqlite3* get_db() { return db; }
    ~DBManager() { if (db) sqlite3_close(db); }
};
