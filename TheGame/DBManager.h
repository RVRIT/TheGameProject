#pragma once
#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <optional>
#include <memory> 

struct User {
    int id;
    std::string username;
    std::string password_hash;
    double rating = 0.0;
    double hours_played = 0.0;
    int games_played = 0;
    int games_won = 0;
};

struct GameSession {
    int id;
    int user_id; // Foreign Key
    int score;
    double duration;
    std::string result; // "Win" or "Loss"
};

inline auto createStorage(const std::string& dbPath) {

    using namespace sqlite_orm;

    return make_storage(dbPath,

        make_table("users",
            make_column("user_id", &User::id, primary_key().autoincrement()),
            make_column("username", &User::username, unique()),
            make_column("password_hash", &User::password_hash),
            make_column("rating", &User::rating),
            make_column("hours_played", &User::hours_played),
            make_column("games_played", &User::games_played),
            make_column("games_won", &User::games_won)
        ),


        make_table("game_sessions",
            make_column("id", &GameSession::id, primary_key().autoincrement()),
            make_column("user_id", &GameSession::user_id),
            make_column("score", &GameSession::score),
            make_column("duration", &GameSession::duration),
            make_column("result", &GameSession::result),
            foreign_key(&GameSession::user_id).references(&User::id))
    );
}

using StorageType = decltype(createStorage("")); //return type for CreateStorage

class DBManager {
public:
    static DBManager& getInstance();

    DBManager(const DBManager&) = delete;
    DBManager& operator=(const DBManager&) = delete;

    bool initialize(const std::string& db_path);
    bool insertGameSession(int user_id, int score);
    bool updateUserStats(int user_id, bool won, double hours_played, int cardsLeftInHand);
    bool checkExistingUser(const std::string& username);
    bool registerUser(const std::string& username, const std::string& hashed_password);
    bool insertGameSession(int user_id, int score, double duration, const std::string& result);
    std::optional<std::string> getHashedPassword(const std::string& username);
    std::optional<int> getUserId(const std::string& username);
    std::optional<User> getUserStats(const std::string& username);


    // Removed Lobby functions (create, join, etc.) from here.
    // We now handle active lobbies in the GameManager (in memory) using a map.
    // The database is only used for Users and saved Game History.




private:
    DBManager() = default;
    std::unique_ptr<StorageType> m_storage; // Smart pointer for orm storage
};  