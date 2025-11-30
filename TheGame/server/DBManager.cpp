#include "DBManager.h"

bool DBManager::initialize(const std::string& db_path)
{
    std::filesystem::create_directories(
        std::filesystem::path(db_path).parent_path()
    );
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "ERROR opening DB: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    const char* create_tables_sql = R"(
    CREATE TABLE IF NOT EXISTS users (
        user_id INTEGER PRIMARY KEY AUTOINCREMENT,
        username TEXT UNIQUE NOT NULL,
        password_hash TEXT NOT NULL, 
        rating REAL DEFAULT 0.0,
        hours_played REAL DEFAULT 0.0,
        games_played INT DEFAULT 0,
        games_won INT DEFAULT 0 
    );
    
    CREATE TABLE IF NOT EXISTS games (
        game_id INTEGER PRIMARY KEY AUTOINCREMENT,
        user_id INTEGER,
        score INTEGER,
        duration REAL,
        result TEXT,
        FOREIGN KEY(user_id) REFERENCES users(user_id)
    );

    CREATE TABLE IF NOT EXISTS lobbies (
        lobby_id INTEGER PRIMARY KEY AUTOINCREMENT,
        game_state TEXT
    );

    CREATE TABLE IF NOT EXISTS lobby_players (
        lobby_id INTEGER,
        user_id INTEGER,
        PRIMARY KEY (lobby_id, user_id),
        FOREIGN KEY(lobby_id) REFERENCES lobbies(lobby_id),
        FOREIGN KEY(user_id) REFERENCES users(user_id)
    );
)";
    char* errmsg;
    if (sqlite3_exec(db, create_tables_sql, nullptr, nullptr, &errmsg) != SQLITE_OK) {
        std::cerr << "SQL ERROR: " << errmsg << std::endl;
        sqlite3_free(errmsg);
        return false;
    }
    return true;
}