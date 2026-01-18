#include "DBManager.h"

using namespace sqlite_orm;


DBManager& DBManager::getInstance() 
{
    static DBManager instance;
    return instance;
}

bool DBManager::initialize(const std::string& db_path)
{
    try {
        std::filesystem::path path(db_path);
        if (path.has_parent_path()) {

            std::filesystem::create_directories(path.parent_path());
        }
        m_storage = std::make_unique<StorageType>(createStorage(db_path));
        m_storage->sync_schema();

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
        m_storage->insert(session);
        return true;
    }
    catch (...) {
        return false;
    }
}

bool DBManager::updateUserStats(int user_id, bool won, double hours_played, int cardsLeftInHand)
{
    try {

        auto user = m_storage->get<User>(user_id);

        // changing values in the memory
        user.games_played++;
        user.hours_played += hours_played;
        if (won) {
            user.games_won++;
        }

        double currentMatchScore = 0.0;

        if (won) {
            
            currentMatchScore = 5.0;
        }

        else {
            
            double penalty = cardsLeftInHand * 0.3;
            currentMatchScore = 4.5 - penalty;

            
            if (currentMatchScore < 1.0) currentMatchScore = 1.0;
        }

        if (user.games_played == 1) {
            
            user.rating = currentMatchScore;
        }
        else {
            
            user.rating = ((user.rating * (user.games_played - 1)) + currentMatchScore) / user.games_played;
        }

        if (user.rating > 5.0) user.rating = 5.0;
        if (user.rating < 1.0) user.rating = 1.0;

       
        m_storage->update(user);
        return true;
    }
    catch (...) {
        return false;
    }
}

bool DBManager::checkExistingUser(const std::string& username)
{
    try {

        auto count = m_storage->count<User>(where(c(&User::username) == username));
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
        m_storage->insert(newUser);
        return true;
    }
    catch (...) {

        return false;
    }
}

std::optional<std::string> DBManager::getHashedPassword(const std::string& username) {

    try {
        // We search for users with this name (there should be at most 1 because of the UNIQUE constraint)    

        auto users = m_storage->get_all<User>(where(c(&User::username) == username));

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
        auto users = m_storage->get_all<User>(where(c(&User::username) == username));

        if (users.empty()) {
            return std::nullopt;
        }
        return users[0].id;
    }
    catch (...) {
        return std::nullopt;
    }
}


bool DBManager::insertGameSession(int user_id, int score, double duration, const std::string& result)
{
    try {
        GameSession session{ -1, user_id, score, duration, result };
        m_storage->insert(session);
        return true;
    }
    catch (...) {
        return false;
    }
}

std::optional<User> DBManager::getUserStats(const std::string& username)
{
    try {
       
        auto users = m_storage->get_all<User>(where(c(&User::username) == username));

        if (users.empty()) {
            return std::nullopt; 
        }

        return users[0]; 
    }
    catch (...) {
        return std::nullopt;
    }
}
std::optional<float> DBManager::getUserRating(const std::string & username)
{
    try
    {
        auto users = m_storage->get_all<User>(where(c(&User::username) == username));
        if (users.empty()) {
            return std::nullopt;
        }
        return users[0].rating;
    }
    catch (...) {
        return std::nullopt;
    }
}
// Lobby functions were removed because we moved the logic to GameManager.
// We don't need to save temporary lobbies in the database anymore



