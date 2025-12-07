#include "lobby.h"
#include <algorithm>

Lobby::Lobby() : maxPlayers(4), status(LobbyStatus::Waiting) {}

Lobby::~Lobby() {}

bool Lobby::AddPlayer(int id, const std::string& name) {
    if (status != LobbyStatus::Waiting || players.size() >= maxPlayers) {
        return false;
    }

    for (const auto& p : players) {
        if (p.id == id) return false;
    }

    players.push_back({ id, name, false });
    return true;
}

void Lobby::RemovePlayer(int id) {
    players.erase(
        std::remove_if(players.begin(), players.end(),
            [id](const PlayerInfo& p) { return p.id == id; }),
        players.end()
    );
}

void Lobby::SetPlayerReady(int id, bool ready) {
    for (auto& p : players) {
        if (p.id == id) {
            p.isReady = ready;
            break;
        }
    }
}

bool Lobby::IsAllReady() const {
    if (players.empty()) return false;
    for (const auto& p : players) {
        if (!p.isReady) return false;
    }
    return true;
}

bool Lobby::SendChatMessage(const std::string& sender, const std::string& content) {
    if (content.empty() || sender.empty()) return false;

    chatHistory.push_back({ sender, content });
    if (chatHistory.size() > MAX_CHAT_MESSAGES) {
        chatHistory.erase(chatHistory.begin());
    }
    return true;
}

std::vector<ChatMessage> Lobby::GetChatHistory() const {
    return chatHistory;
}

GameSnapshot Lobby::CreateGameSnapshot() {
    GameSnapshot snapshot;

    if (!IsAllReady()) {
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

const std::vector<PlayerInfo>& Lobby::GetPlayers() const {
    return players;
}

LobbyStatus Lobby::GetStatus() const {
    return status;
}

void Lobby::SetMaxPlayers(int max) {
    maxPlayers = max;
}

json Lobby::GetStateJSON() const {
    json j;
    j["status"] = (status == LobbyStatus::Waiting ? "Waiting" :
        (status == LobbyStatus::InProgress ? "InProgress" : "Finished"));
    j["maxPlayers"] = maxPlayers;

    j["players"] = players;
    j["chat"] = chatHistory;

    return j;
}