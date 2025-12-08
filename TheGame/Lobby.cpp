#include "lobby.h"
#include <algorithm>
bool Lobby::addPlayer(const std::string& name) {
    if (status != LobbyStatus::Waiting || players.size() >= MAX_PLAYERS) {
        return false;
    }
    int newId = static_cast<int>(players.size());
    players.push_back({newId, name, false});
    return true;
}

void Lobby::removePlayer(int id) {
    players.erase(
        std::remove_if(players.begin(), players.end(),
            [id](const PlayerInfo& p) { return p.id == id; }),
        players.end()
    );
}

void Lobby::setPlayerReady(int id, bool ready) {
    for (auto& p : players) {
        if (p.id == id) {
            p.isReady = ready;
            break;
        }
    }
}

bool Lobby::isAllReady() const {
    if (players.empty()) return false;
    for (const auto& p : players) {
        if (!p.isReady) return false;
    }
    return true;
}

bool Lobby::sendChatMessage(const std::string& sender, const std::string& content) {
    if (content.empty() || sender.empty()) return false;

    chatHistory.push_back({ sender, content });
    if (chatHistory.size() > MAX_CHAT_MESSAGES) {
        chatHistory.erase(chatHistory.begin());
    }
    return true;
}

std::vector<ChatMessage> Lobby::getChatHistory() const {
    return chatHistory;
}

GameSnapshot Lobby::CreateGameSnapshot() {
    GameSnapshot snapshot;

    if (!isAllReady()) {
        snapshot.isGameOver = false;
        return snapshot;
    }

    status = LobbyStatus::InProgress;

    for (const auto& p : players) {
        OtherPlayerInfo opi;
        opi.name = p.name;
        opi.cardCount = 0;
        opi.canMakeMove = false;
        opi.isCurrentPlayer = false;
        snapshot.opponents.push_back(opi);
    }

    snapshot.deckSize = 52;
    snapshot.message = "Game Started";
    snapshot.isGameOver = false;

    return snapshot;
}

const std::vector<PlayerInfo>& Lobby::getPlayers() const {
    return players;
}

LobbyStatus Lobby::getStatus() const {
    return status;
}


crow::json::wvalue Lobby::getStateJSON() const {
    crow::json::wvalue j;

    // Status
    if (status == LobbyStatus::Waiting) {
        j["status"] = "Waiting";
    }
    else if (status == LobbyStatus::InProgress) {
        j["status"] = "InProgress";
    }
    else {
        j["status"] = "Finished";
    }

    j["MAX_PLAYERS"] = static_cast<int>(MAX_PLAYERS);

    crow::json::wvalue playersArr;
    for (size_t i = 0; i < players.size(); ++i) {
        playersArr[i]["id"] = players[i].id;
        playersArr[i]["name"] = players[i].name;
        playersArr[i]["isReady"] = players[i].isReady;
    }
    j["players"] = std::move(playersArr);
    crow::json::wvalue chatArr;
    for (size_t i = 0; i < chatHistory.size(); ++i) {
        chatArr[i]["sender"] = chatHistory[i].sender;
        chatArr[i]["content"] = chatHistory[i].content;
    }
    j["chat"] = std::move(chatArr);

    return j;
}