#include "lobby.h"
#include <algorithm>
#include <regex>
Lobby::Lobby(int id, const std::string& hostName, float hostRating, LobbyStatus m_status) 
    : id{ id }, m_status{ m_status }
{
    addPlayer(hostName, hostRating);
}

bool Lobby::addPlayer(const std::string& name, float playerRating) {
    if (m_status != LobbyStatus::Waiting || m_players.size() >= MAX_PLAYERS) {
        return false;
    }
    int newId = static_cast<int>(m_players.size());
    m_players.push_back({ newId, name, false, playerRating });
    return true;
}

void Lobby::removePlayer(int id) {
    m_players.erase(
        std::remove_if(m_players.begin(), m_players.end(),
            [id](const PlayerInfo& p) { return p.id == id; }),
        m_players.end()
    );
}

bool Lobby::setPlayerReady(int id, bool ready) {
    for (auto& p : m_players) {
        if (p.id == id) {
            p.isReady = ready;
            return true;
        }
    }
    return false;
}

bool Lobby::isAllReady() const {
    if (m_players.empty()) return false;
    for (const auto& p : m_players) {
        if (!p.isReady) return false;
    }
    return true;
}

bool Lobby::sendChatMessage(const std::string& sender, const std::string& content) {
    if (content.empty() || sender.empty()) return false;

    std::regex numberPattern(R"(\b\d+\b)");
    std::string filteredContent = std::regex_replace(content, numberPattern, "[#]");

    m_chatHistory.push_back({ sender, filteredContent });
    if (m_chatHistory.size() > MAX_CHAT_MESSAGES) {
        m_chatHistory.erase(m_chatHistory.begin());
    }
    return true;
}
bool Lobby::tryStartGame(int requestPlayerId) {
    if (m_players.size() < 2) {
        return false;
    }

    if (m_players.empty() || m_players[0].id != requestPlayerId) {
        return false;
    }

    if (!isAllReady()) {
        return false;
    }

    std::vector<std::string_view> namesView;
    for (const auto& p : m_players) namesView.push_back(p.name);

    m_game = std::make_unique<Game>(namesView);
    m_status = LobbyStatus::InProgress;

    m_status = LobbyStatus::InProgress;

    return true;
}

std::vector<ChatMessage> Lobby::getChatHistory() const {
    return m_chatHistory;
}

GameSnapshot Lobby::CreateGameSnapshot() {
    GameSnapshot snapshot;

    if (!isAllReady()) {
        snapshot.isGameOver = false;
        return snapshot;
    }

    m_status = LobbyStatus::InProgress;

    for (const auto& p : m_players) {
        OtherPlayerInfo opi;
        opi.name = p.name;
        opi.cardCount = 0;
        opi.canMakeMove = false;
        opi.isCurrentPlayer = false;
        snapshot.players.push_back(opi);
    }

    snapshot.deckSize = 52;
    snapshot.message = "Game Started";
    snapshot.isGameOver = false;

    return snapshot;
}

Game* Lobby::getGame()
{
    return m_game.get();
}


const std::vector<PlayerInfo>& Lobby::getPlayers() const {
    return m_players;
}

LobbyStatus Lobby::getStatus() const {
    return m_status;
}

float Lobby::getAverageRating() const {
    if (m_players.empty()) return 0.0f;

    double sum = 0.0;
    for (const auto& p : m_players) sum += p.rating;

    return static_cast<float>(sum / m_players.size());
}

crow::json::wvalue Lobby::getStateJson() const {
    crow::json::wvalue j;

    if (m_status == LobbyStatus::Waiting) {
        j["status"] = "Waiting";
    }
    else if (m_status == LobbyStatus::InProgress) {
        j["status"] = "InProgress";
    }
    else {
        j["status"] = "Finished";
    }

    j["MAX_PLAYERS"] = static_cast<int>(MAX_PLAYERS);

    crow::json::wvalue playersArr;
    for (size_t i = 0; i < m_players.size(); ++i) {
        playersArr[i]["id"] = m_players[i].id;
        playersArr[i]["name"] = m_players[i].name;
        playersArr[i]["isReady"] = m_players[i].isReady;
        playersArr[i]["rating"] = m_players[i].rating;  
    }
    j["avgRating"] = getAverageRating();
    j["players"] = std::move(playersArr);
    crow::json::wvalue chatArr;
    for (size_t i = 0; i < m_chatHistory.size(); ++i) {
        chatArr[i]["sender"] = m_chatHistory[i].sender;
        chatArr[i]["content"] = m_chatHistory[i].content;
    }
    j["chat"] = std::move(chatArr);

    return j;

}
bool Lobby::isPlayerInLobby(const std::string& playerName)
{
    for (auto& p : m_players)
    {
        if (playerName == p.name)
        {
            return true;
        }
    }
    return false;
}
void Lobby::resetGame() {
    m_game.reset();
    m_status = LobbyStatus::Waiting;
    for (auto& p : m_players) {
        p.isReady = false;
    }
}
