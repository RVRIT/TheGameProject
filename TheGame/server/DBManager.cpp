#include "DBManager.h"


using namespace sqlite_orm;

bool DBManager::initialize(const std::string& db_path)
{

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
    try {
        
        auto count = storage->count<User>(where(c(&User::username) == username));
        return count > 0;
    }
    catch (...) {
        return false;
    }
}

bool DBManager::registerUser(const std::string& username, const std::string& hashed_password)
{    
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
    try {
        auto users = storage->get_all<User>(where(c(&User::username) == username));

        if (users.empty()) {
            return std::nullopt;
        }
        return users[0].id;
    }
    catch (...) {
        return std::nullopt;
    }
}

std::optional<std::string> DBManager::getGameState(const int& lobbyId)
{
   
    try {
        
        auto lobbies = storage->get_all<LobbyDb>(where(c(&LobbyDb::id) == lobbyId));

        if (lobbies.empty()) {
            return std::nullopt;
        }
        return lobbies[0].game_state;
    }
    catch (...) {
        return std::nullopt;
    }
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