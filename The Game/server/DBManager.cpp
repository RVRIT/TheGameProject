#include "DBManager.h"
#include "PasswordService.h"
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
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT UNIQUE NOT NULL,
                password_hash TEXT NOT NULL, 
                rating REAL DEFAULT 0.0,
                hours_played REAL DEFAULT 0.0,
                games_played INT DEFAULT 0,
                games_won INT DEFAULT 0 
            );
            
            CREATE TABLE IF NOT EXISTS game_sessions (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                user_id INTEGER,
                score INTEGER,
                duration REAL,
                result TEXT,
                FOREIGN KEY(user_id) REFERENCES users(id)
            );
        )";
    char* errmsg;
    if (sqlite3_exec(db, create_tables_sql, nullptr, nullptr, &errmsg) != SQLITE_OK) {
        std::cerr << "SQL ERROR: " << errmsg << std::endl;
        sqlite3_free(errmsg);
        return false;
    }
    std::cout << "Database initialized successfully!" << std::endl;
    return true;
}
bool DBManager::insertGameSession(int user_id, int score)
{
    const char* sql = "INSERT INTO game_sessions (user_id, score) VALUES (?, ?)";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "ERROR preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, score);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "ERROR insert execution: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
bool DBManager::updateUserStats(int user_id, bool won, double hours_played)
{
    const char* sql = R"(
        UPDATE users 
        SET games_played = games_played + 1,
            hours_played = hours_played + ?,
            games_won = games_won + ?
        WHERE id = ?
    )";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_double(stmt, 1, hours_played);
    sqlite3_bind_int(stmt, 2, won ? 1 : 0);
    sqlite3_bind_int(stmt, 3, user_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}
bool DBManager::checkExistingUser(const std::string& username)
{
    if (!db)
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
    else {
        std::cerr << "ERROR SQL: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    return success;
}
bool DBManager::registerUser(const std::string& username, const std::string& hashed_password) {
    const char* sql = "INSERT INTO users (username, password_hash) VALUES (?, ?)";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hashed_password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}
std::optional<std::string> DBManager::getHashedPassword(const std::string& username) {
    if (!db) return std::nullopt;

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
    return result;
}

std::optional<int> DBManager::getUserId(const std::string& username) {
    if (!db) return std::nullopt;

    const char* sql = "SELECT id FROM users WHERE username LIKE ?";
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