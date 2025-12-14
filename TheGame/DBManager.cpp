#include "DBManager.h"


using namespace sqlite_orm;

bool DBManager::initialize(const std::string& db_path)
{

    try {

        std::filesystem::path path(db_path);
        if (path.has_parent_path()) {

            std::filesystem::create_directories(path.parent_path());
        }


        storage = std::make_unique<StorageType>(createStorage(db_path));
        storage->sync_schema();

        std::cout << "DB initialized successfully at: " << db_path << std::endl;
        return true;
    }
    catch (std::system_error& e) {
        std::cerr << "Eroare la initializare DB: " << e.what() << std::endl;
        return false;
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
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

