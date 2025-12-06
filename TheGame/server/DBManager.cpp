#include "DBManager.h"


using namespace sqlite_orm;

bool DBManager::initialize(const std::string& db_path)
{
    /*std::filesystem::create_directories(
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
    return true;*/
    try {
        
        storage = std::make_unique<StorageType>(createStorage(db_path));

       
        storage->sync_schema();
        return true;
    }
    catch (std::system_error& e) {
        std::cerr << "Eroare la initializare DB: " << e.what() << std::endl;
        return false;
    }
}

bool DBManager::insertGameSession(int user_id, int score)
{
    try {
        
        GameSession session{ -1, user_id, score, 0.0, "" }; // duration 0, result empty for now
        storage->insert(session);
        return true;
    }
    catch (...) {
        return false;
    }
}

bool DBManager::updateUserStats(int user_id, bool won, double hours_played)
{
    /*const char* sql = R"(
        UPDATE users 
        SET games_played = games_played + 1,
            hours_played = hours_played + ?,
            games_won = games_won + ?
        WHERE user_id = ?
    )";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_double(stmt, 1, hours_played);
    sqlite3_bind_int(stmt, 2, won ? 1 : 0);
    sqlite3_bind_int(stmt, 3, user_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;*/

    try {
       
        auto user = storage->get<User>(user_id); 

        // changing values in the memory
        user.games_played++;
        user.hours_played += hours_played;
        if (won) {
            user.games_won++;
        }

        
        storage->update(user); //sending the update back to the database
        return true;
    }
    catch (...) {
        return false;
    }
}

bool DBManager::checkExistingUser(const std::string& username)
{
    /*if (!db)
    {
        return false;
    }
    sqlite3_stmt* stmt;
    bool success = false;
    const char* sql = "SELECT COUNT(*) FROM users WHERE username LIKE ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int count = sqlite3_column_int(stmt, 0);
            success = (count > 0);
        }
    }
    sqlite3_finalize(stmt);
    return success;*/

    try {
        
        auto count = storage->count<User>(where(c(&User::username) == username));
        return count > 0;
    }
    catch (...) {
        return false;
    }
}

bool DBManager::registerUser(const std::string& username, const std::string& hashed_password) {
    try {
        // ID -1 for auto increment

        User newUser{ -1, username, hashed_password, 0.0, 0.0, 0, 0 };
        storage->insert(newUser);
        return true;
    }
    catch (...) {
        
        return false;
    }
}

std::optional<std::string> DBManager::getHashedPassword(const std::string& username) {
   /* if (!db) return std::nullopt;

    const char* sql = "SELECT password_hash FROM users WHERE username LIKE ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    std::optional<std::string> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (hash) result = std::string(hash);
    }
    sqlite3_finalize(stmt);
    return result;*/

    try {
        // We search for users with this name (there should be at most 1 because of the UNIQUE constraint)    
        
         auto users = storage->get_all<User>(where(c(&User::username) == username));

        if (users.empty()) {
            return std::nullopt;
        }
        return users[0].password_hash;
    }
    catch (...) {
        return std::nullopt;
    }
}

std::optional<int> DBManager::getUserId(const std::string& username) {
    if (!db) return std::nullopt;

    const char* sql = "SELECT user_id FROM users WHERE username LIKE ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    std::optional<int> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return result;
}

std::optional<std::string> DBManager::getGameState(const int& lobbyId)
{
    if (!db) return std::nullopt;

    const char* sql = "SELECT game_state FROM lobbies WHERE lobby_id = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_int(stmt, 1, lobbyId);
    std::optional<std::string> result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (text) {
            result = std::string(reinterpret_cast<const char*>(text));
        }

    }
    sqlite3_finalize(stmt);
    return result;
}

int DBManager::createLobby(int user_id)
{
    if (!db) return -1;

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    const char* sqlLobby = "INSERT INTO lobbies (game_state) VALUES ('WAITING')";
    sqlite3_stmt* stmtLobby;

    if (sqlite3_prepare_v2(db, sqlLobby, -1, &stmtLobby, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return -1;
    }

    if (sqlite3_step(stmtLobby) != SQLITE_DONE) {
        sqlite3_finalize(stmtLobby);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return -1;
    }
    sqlite3_finalize(stmtLobby);

    int lobby_id = (int)sqlite3_last_insert_rowid(db);

    const char* sqlPlayer = "INSERT INTO lobby_players (lobby_id, user_id) VALUES (?, ?)";
    sqlite3_stmt* stmtPlayer;

    if (sqlite3_prepare_v2(db, sqlPlayer, -1, &stmtPlayer, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return -1;
    }

    sqlite3_bind_int(stmtPlayer, 1, lobby_id);
    sqlite3_bind_int(stmtPlayer, 2, user_id);

    if (sqlite3_step(stmtPlayer) != SQLITE_DONE) {
        sqlite3_finalize(stmtPlayer);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return -1;
    }
    sqlite3_finalize(stmtPlayer);

    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

    return lobby_id;
}

bool DBManager::joinLobby(int user_id, int lobby_id)
{
    if (!db) return false;

    const char* sql = "INSERT INTO lobby_players (lobby_id, user_id) VALUES (?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, lobby_id);
    sqlite3_bind_int(stmt, 2, user_id);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DBManager::leaveLobby(int user_id)
{
    if (!db) return false;

    const char* sql = "DELETE FROM lobby_players WHERE user_id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}