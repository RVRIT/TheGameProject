#pragma once
#include <sqlite3.h>
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

struct LobbyDb {
    int id;
    std::string game_state; // "WAITING", "PLAYING"
};

struct LobbyPlayer {
    int id;
    int lobby_id;
    int user_id;
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
            foreign_key(&GameSession::user_id).references(&User::id)
        ),

        
        make_table("lobbies",
            make_column("lobby_id", &LobbyDb::id, primary_key().autoincrement()),
            make_column("game_state", &LobbyDb::game_state)
        ),

       
        make_table("lobby_players",
            make_column("id", &LobbyPlayer::id, primary_key().autoincrement()),
            make_column("lobby_id", &LobbyPlayer::lobby_id),
            make_column("user_id", &LobbyPlayer::user_id),
            foreign_key(&LobbyPlayer::lobby_id).references(&LobbyDb::id),
            foreign_key(&LobbyPlayer::user_id).references(&User::id)
        )
    );
}

using StorageType = decltype(createStorage("")); //return type for CreateStorage

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
    DBManager() = default;

    // no MORE sqlite3* db!
    
    std::unique_ptr<StorageType> storage; // Smart pointer for orm storage
};