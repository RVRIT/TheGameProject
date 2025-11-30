#pragma once
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <optional>

class DBManager {
public:
    static DBManager& getInstance() {
        static DBManager instance;
        return instance;
    }

    DBManager(const DBManager&) = delete;
    DBManager& operator=(const DBManager&) = delete;

    bool initialize(const std::string& db_path);
    bool insertGameSession(int user_id, int score);
    bool updateUserStats(int user_id, bool won, double hours_played);
    bool checkExistingUser(const std::string& username);
    bool registerUser(const std::string& username, const std::string& hashed_password);
    std::optional<std::string> getHashedPassword(const std::string& username);
    std::optional<int> getUserId(const std::string& username);
    std::optional<std::string> getGameState(const int& lobbyId);

    int createLobby(int user_id);
    bool joinLobby(int user_id, int lobby_id);
    bool leaveLobby(int user_id);

private:
    DBManager() : db(nullptr) {}
    ~DBManager() {
        if (db) {
            sqlite3_close(db);
        }
    }

    sqlite3* db;
};